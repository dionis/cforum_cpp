/*!
 * \file cgi.c
 * \author Christian Kruse, <cjk@wwwtech.de>
 * \brief the implementation of the Classic Forum CGI library
 *
 * a small (really small!), but hopefully fast cgi-library. It can only
 * parse x-www-form/url-encoded
 */

#include "cgi.h"


/*!
 * This function parses a given parameter string into name-value pairs.
 * \param hash the CGI hash
 * \param data the parameter string
 */
static int _cf_cgi_parse_params(cf_hash_t *hash,char *data);

/*!
 * this function saves a parsed parameter to the CGI hash.
 * \param hash    the CGI hash
 * \param name    the name of the parameter
 * \param namlen  the length of the name string
 * \param value   the value of the parameter
 */
static int _cf_cgi_save_param(cf_hash_t *hash,char *name,int namlen,char *value);


cf_hash_t *cf_cgi_new(void) {
  char *clen = getenv("CONTENT_LENGTH"),*rqmeth = getenv("REQUEST_METHOD"),*data;
  cf_hash_t *hash;
  long len  = 0;
  int trash = 0;

  if(!rqmeth) return NULL;
  if(clen) len = strtol(clen,NULL,10);

  hash = cf_hash_new(cf_cgi_destroy_entry);

  if(strcmp(rqmeth,"GET") == 0) {
    data = getenv("QUERY_STRING");
  }
  else {
    if(!len) {
      cf_hash_destroy(hash);
      free(hash);
      return NULL;
    }

    trash = 1;
    data  = cf_alloc(NULL,len + 1,1,CF_ALLOC_MALLOC);

    fread(data,1,len,stdin);
    data[len] = '\0';
  }

  if(data && *data) {
    if(!_cf_cgi_parse_params(hash,data)) {
      cf_hash_destroy(hash);
      free(hash);
      if(trash) free(data);
      return NULL;
    }

    return hash;
  }
  else {
    cf_hash_destroy(hash);
    free(hash);
  }

  return NULL;
}

void cf_cgi_parse_path_info(cf_array_t *ary) {
  char *data = getenv("PATH_INFO");
  register char *ptr = (char *)data+1;
  char *start = data,*name = NULL;

  cf_array_init(ary,sizeof(char **),NULL);

  if(!data) return;

  if(*ptr) {
    for(;*ptr;ptr++) {
      if(*ptr == '/') {
        if(start) {
          name = strndup(start+1,ptr-start-1);
          cf_array_push(ary,&name);
          start = ptr;
        }
        else {
          start = ptr;
        }
      }
    }

    if(*start != '/') {
      name = strdup(start+1);
      cf_array_push(ary,&name);
    }
  }
}

cf_hash_t *cf_cgi_parse_path_info_nv(cf_hash_t *hash) {
  char *pi = getenv("PATH_INFO"),*start,*name,*value;
  register char *ptr;

  start = name = value = NULL;

  if(pi) {
    if(hash == NULL) hash = cf_hash_new(cf_cgi_destroy_entry);

    for(ptr=pi;*ptr;ptr++) {
      if(*ptr == '/') {
        if(start == NULL) start = ptr;
        else {
          if(name == NULL) {
            *ptr = '\0';
            name = cf_cgi_url_decode(start+1,ptr-start);
            *ptr = '/';
          }
          else {
            *ptr  = '\0';
            value = cf_cgi_url_decode(start+1,ptr-start);
            *ptr  = '/';

            _cf_cgi_save_param(hash,name,strlen(name),value);

            name = value = NULL;
          }

          start = ptr;
        }
      }
    }

    if(*(ptr-1) != '/') {
      value = cf_cgi_url_decode(start+1,ptr-start-1);
      _cf_cgi_save_param(hash,name,strlen(name),value);
    }
  }

  return hash;
}

void cf_cgi_parse_cookies(cf_hash_t *hash) {
  char *cookies = getenv("HTTP_COOKIE");
  char *pos = cookies,*pos1 = cookies;
  char *name = NULL,*value = NULL;
  int len = 0,namlen = 0,vallen = 0;

  if(cookies) {
    while((pos = strstr(pos1,"=")) != NULL) {
      for(;*pos && isspace(*pos);++pos);

      namlen = pos - pos1;
      *pos   = 0;
      name   = cf_cgi_url_decode(pos1,namlen);
      *pos   = '=';

      pos1 = strstr(pos,";");
      if(!pos1) break;

      vallen  = pos1 - pos;
      *pos1   = 0;
      value   = cf_cgi_url_decode(pos+1,vallen);
      *pos1++ = ';';

      for(pos = name;*pos && isspace(*pos);++pos);

      if(!_cf_cgi_save_param(hash,name,namlen,value)) {
        free(name);
        free(value);

        fprintf(stderr,"%s[%d]: out of memory!\n",__FILE__,__LINE__);
        return;
      }
    }

    if(pos && *pos) {
      len   = strlen(pos+1);
      value = cf_cgi_url_decode(pos+1,len);

      for(pos = name;*pos && isspace(*pos);++pos);

      if(!_cf_cgi_save_param(hash,pos,namlen,value)) {
        free(name);
        free(value);

        fprintf(stderr,"%s[%d]: out of memory!\n",__FILE__,__LINE__);
        return;
      }
    }
  }

}

char *cf_cgi_url_decode(const char *str,size_t len) {
  char *ret = cf_alloc(NULL,len + 1,1,CF_ALLOC_MALLOC); /* the new string can be as long as the old (but not longer) */
  register char *ptr1,*ptr2;
  char ptr3[3] = { '\0','\0','\0' };

  if(!ret) return NULL;

  for(ptr1=ret,ptr2=(char *)str;*ptr2;ptr2++,ptr1++) {
    switch(*ptr2) {
      case '+':
        *ptr1 = ' ';
        break;
      case '%':
        memcpy(ptr3,ptr2+1,2);
        *ptr1 = strtol(ptr3,NULL,16);
        ptr2 += 2;
        break;
      default:
        *ptr1 = *ptr2;
    }
  }

  *ptr1 = '\0';
  return ret;
}

char *cf_cgi_url_encode(const char *str,size_t len) {
  long nlen = 3 * len + 1; /* new string can max 3x longer than old string (plus 0-byte) */
  char *nstr = cf_alloc(NULL,nlen,1,CF_ALLOC_MALLOC);
  register char *ptr1,*ptr2;

  for(ptr1=(char *)str,ptr2=nstr;*ptr1;ptr2++,ptr1++) {
    if((*ptr1 >= 48 && *ptr1 <= 122) || (*ptr1 == '_' || *ptr1 == '.' || *ptr1 == '-')) {
      *ptr2 = *ptr1;
    }
    else {
      if(*ptr1 == ' ') {
        *ptr2 = '+';
      }
      else {
        sprintf(ptr2,"%%%02X",(int)*ptr1);
        ptr2 += 2;
      }
    }
  }

  *ptr2 = '\0';

  return nstr;
}

static int _cf_cgi_save_param(cf_hash_t *hash,char *name,int namlen,char *value) {
  cf_cgi_param_t *ent,*ent1;

  namlen = strlen(name);

  ent = cf_hash_get(hash,name,namlen);
  if(!ent) {
    ent        = cf_alloc(NULL,1,sizeof(cf_cgi_param_t),CF_ALLOC_CALLOC);

    ent->name  = name;
    ent->value = value;

    cf_hash_set(hash,name,namlen,ent,sizeof(cf_cgi_param_t));

    /* we make a copy, so we do not need it again */
    free(ent);

    ent = cf_hash_get(hash,name,strlen(name));
  }
  else {
    ent1            = cf_alloc(NULL,1,sizeof(cf_cgi_param_t),CF_ALLOC_CALLOC);

    ent1->name  = name;
    ent1->value = value;

    if(ent->last)
      ent->last->next = ent1;
    else
      ent->next       = ent1;

    ent->last         = ent1;
  }

  return 1;
}

static int _cf_cgi_parse_params(cf_hash_t *hash,char *data) {
  char  *pos = data,*pos1 = data;
  char *name = NULL,*value = NULL;
  int len = 0,namlen = 0,vallen = 0;

  while((pos = strstr(pos1,"=")) != NULL) {
    namlen = pos - pos1;
    *pos   = 0;
    name   = cf_cgi_url_decode(pos1,namlen);
    *pos   = '=';

    pos1 = strstr(pos,"&");
    if(!pos1) break;

    vallen  = pos1 - pos;
    *pos1   = 0;
    value   = cf_cgi_url_decode(pos+1,vallen);
    *pos1++ = '&';

    if(!_cf_cgi_save_param(hash,name,namlen,value)) {
      free(name);
      free(value);

      fprintf(stderr,"%s[%d]: out of memory!\n",__FILE__,__LINE__);
      return 0;
    }
  }

  if(pos && *pos) {
    len   = strlen(pos+1);
    value = cf_cgi_url_decode(pos+1,len);

    if(!_cf_cgi_save_param(hash,name,namlen,value)) {
      free(name);
      free(value);

      fprintf(stderr,"%s[%d]: out of memory!\n",__FILE__,__LINE__);
      return 0;
    }
  }

  return 1;
}

void cf_cgi_destroy_entry(void *data) {
  cf_cgi_param_t *ent = (cf_cgi_param_t *)data,*ent1;

  for(;ent;ent=ent1) {
    free(ent->name);
    free(ent->value);

    ent1 = ent->next;

    if(ent != data) free(ent);
  }
}

char *cf_cgi_get(cf_hash_t *hash,char *name) {
  cf_cgi_param_t *p = cf_hash_get(hash,name,strlen(name));

  if(p) return p->value;

  return NULL;
}

void cf_cgi_set(cf_hash_t *hash,const char *name,const char *value) {
  size_t nlen = strlen(name);
  cf_cgi_param_t *p = cf_hash_get(hash,(char *)name,nlen),*p1,*p2;
  cf_cgi_param_t par;

  if(p) {
    if(p->next) {
      for(p1=p->next;p1;p1=p2) {
        free(p1->name);
        free(p1->value);
        p2 = p1->next;
        free(p1);
      }
    }

    free(p->value);
    p->value = strdup(value);
  }
  else {
    par.name  = strdup(name);
    par.value = strdup(value);
    par.next  = NULL;
    par.last  = NULL;

    cf_hash_set(hash,(char *)name,nlen,&par,sizeof(par));
  }
}

cf_cgi_param_t *cf_cgi_get_multiple(cf_hash_t *hash,char *param) {
  return (cf_cgi_param_t *)cf_hash_get(hash,param,strlen(param));
}

void cf_cgi_destroy(cf_hash_t *hash) {
  cf_hash_destroy(hash);
  free(hash);
}

u_int32_t cf_cgi_path_info_parsed(char ***infos) {
  char *path = getenv("PATH_INFO"),*prev,**list = NULL;
  register char *ptr;
  int len = 0;

  if(path) {
    for(prev=ptr=path+1;*ptr;ptr++) {
      if(*ptr == '/') {
        *ptr        = '\0';
        list        = cf_alloc(list,++len,sizeof(char **),CF_ALLOC_REALLOC);
        list[len-1] = cf_cgi_url_decode(prev,ptr-prev);
        prev        = ptr+1;
        *ptr        = '/';
      }
    }
  }

  *infos = list;
  return len;
}

/* eof */
