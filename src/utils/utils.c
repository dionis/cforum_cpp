/*!
 * \file utils.c
 * \brief Utilities for the Classic Forum, making the life easier
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * Utilities for the Classic Forum, such as getline(), make_path()
 * remove_path(), etc, pp making the life easier
 */

#include "utils.h"

int cf_strcasecmp(const char *str1,const char *str2) {
  register const char *ptr1 = str1,*ptr2 = str2;

  while(*ptr1 && *ptr2) {
    if(g_unichar_toupper(g_utf8_get_char(ptr1)) != g_unichar_toupper(g_utf8_get_char(ptr2))) return -1;

    ptr1 = g_utf8_next_char(ptr1);
    ptr2 = g_utf8_next_char(ptr2);
  }

  return 0;
}

int cf_strncasecmp(const char *str1,const char *str2,size_t n) {
  register const char *ptr1 = str1,*ptr2 = str2;
  register size_t i;

  for(i=0;*ptr1 && *ptr2 && i < n && g_unichar_toupper(g_utf8_get_char(ptr1)) == g_unichar_toupper(g_utf8_get_char(ptr2));ptr1 = g_utf8_next_char(ptr1),ptr2 = g_utf8_next_char(ptr2),i++) {
    if(i == n - 1) return 0;
  }

  return -1;
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define ONEMASK ((size_t)(-1) / 0xFF)
#endif

size_t cf_strlen_utf8(const char *_s) {
  const char * s;
  size_t count = 0;
  size_t u;
  unsigned char b;

  /* Handle any initial misaligned bytes. */
  for(s = _s; (uintptr_t)(s) & (sizeof(size_t) - 1); s++) {
    b = *s;

    /* Exit if we hit a zero byte. */
    if(b == '\0') goto done;

    /* Is this byte NOT the first byte of a character? */
    count += (b >> 7) & ((~b) >> 6);
  }

  /* Handle complete blocks. */
  for(; ;s+=sizeof(size_t)) {
    /* Prefetch 256 bytes ahead. */
    __builtin_prefetch(&s[256], 0, 0);

    /* Grab 4 or 8 bytes of UTF-8 data. */
    u = *(size_t *)(s);

    /* Exit the loop if there are any zero bytes. */
    if((u - ONEMASK) & (~u) & (ONEMASK * 0x80)) break;

    /* Count bytes which are NOT the first byte of a character. */
    u = ((u & (ONEMASK * 0x80)) >> 7) & ((~u) >> 6);
    count += (u * ONEMASK) >> ((sizeof(size_t) - 1) * 8);
  }

  /* Take care of any left-over bytes. */
  for(; ;++s) {
    b = *s;

    /* Exit if we hit a zero byte. */
    if(b == '\0') break;

    /* Is this byte NOT the first byte of a character? */
    count += (b >> 7) & ((~b) >> 6);
  }

  done:
  return ((s - _s) - count);
}


int cf_remove_path(const char *path) {
  DIR *dir;
  struct dirent *ent = NULL,*entry = NULL;
  char *buff;
  struct stat st;
  size_t len = strlen(path);
  int ret;

  if(lstat(path, &st) < 0) {
    fprintf(stderr,"utils: cf_remove_path: could not lstat '%s': %s\n",path,strerror(errno));
    return -1;
  }

  if(S_ISDIR(st.st_mode)) {
    if((dir = opendir(path)) == NULL) {
      fprintf(stderr,"utils: cf_remove_path: could not opendir '%s': %s\n",path,strerror(errno));
      return -1;
    }

    while((readdir_r(dir,entry,&ent)) == 0) {
      /* take care of . and .. */
      if(strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0) continue;

      /* Recursively call to remove the current entry */
      buff = cf_alloc(NULL,1,len + strlen(ent->d_name) + 2,CF_ALLOC_MALLOC);
      sprintf(buff,"%s/%s",path,ent->d_name);
      ret = cf_remove_path(buff);
      free(buff);

      if(ret != 0) return -1;
    }

    if(rmdir(path) < 0) {
      fprintf(stderr,"utils: cf_remove_path: could not rmdir '%s': %s\n",path,strerror(errno));
      return -1;
    }
  }
  else {
    /* path is not a directory; just unlink() it */
    if(unlink(path) < 0) {
      fprintf(stderr,"utils: cf_remove_path: could not unlink '%s': %s\n",path,strerror(errno));
      return -1;
    }
  }

  return 0;
}


int cf_make_path(const char *path,mode_t mode) {
  char *mpath = strdup(path);
  register char *ptr;

  for(ptr=mpath+1;*ptr;++ptr) {
    if(*ptr == '/') {
      *ptr = '\0';

      if(mkdir(mpath,mode) != 0) {
        if(errno != EEXIST) return -1;
      }

      *ptr = '/';
    }
  }

  if(*(ptr-1) != '/') {
    if(mkdir(mpath,mode) != 0) {
      if(errno != EEXIST) return -1;
    }
  }

  return 0;
}

#ifdef HAS_NO_GETLINE
size_t getline(char **lineptr,size_t *n,FILE *stream) {
  return getdelim(lineptr,n,'\n',stream);
}
#endif

#ifdef HAS_NO_GETDELIM
size_t getdelim(char **lineptr,size_t *n,int delim,FILE *stream) {
  GString buf = {NULL,0,0};
  register char c;

  while(!feof(stream) && !ferror(stream)) {
    c = fgetc(stream);
    g_string_append_c(&buf,c);

    if(c == delim) break;
  }

  if(*lineptr) free(*lineptr);

  *lineptr = buf.str;
  *n       = buf.len;

  if(feof(stream) || ferror(stream)) return -1;

  return buf.len;
}
#endif

#ifdef NOSTRDUP
char *strdup(const char *str) {
  size_t len = strlen(str);
  char *buff = cf_alloc(NULL,1,len+1,CF_ALLOC_MALLOC);

  memcpy(buff,str,len+1);

  return buff;
}
#endif

#ifdef NOSTRNDUP
char *strndup(const char *str,size_t len) {
  char *buff = cf_alloc(NULL,1,len+1,CF_ALLOC_MALLOC);

  memcpy(buff,str,len);
  buff[len-1] = '\0';

  return buff;
}
#endif

size_t split(const char *big,const char *small,char ***ulist) {
  char **list  = cf_alloc(NULL,CF_PRERESERVE,sizeof(*list),CF_ALLOC_MALLOC);
  char  *pos   = (char *)big,*pre = (char *)big;
  size_t len   = 0;
  size_t reser = CF_PRERESERVE;
  size_t slen  = strlen(small);

  while((pos = strstr(pos,small)) != NULL) {
    *pos = '\0';

    list[len++] = strdup(pre);

    if(len >= reser) {
      reser += CF_PRERESERVE;
      list = cf_alloc(list,reser,sizeof(*list),CF_ALLOC_REALLOC);
    }

    pre    = pos+slen;
    *pos++ = *small;
  }

  if(len >= reser) list = cf_alloc(list,++reser,sizeof(*list),CF_ALLOC_REALLOC);
  list[len++] = strdup(pre);

  *ulist = list;

  return len;
}

size_t nsplit(const char *big,const char *small,char ***ulist,size_t max) {
  char **list  = cf_alloc(NULL,CF_PRERESERVE,sizeof(*list),CF_ALLOC_MALLOC);
  char  *pos   = (char *)big,*pre = (char *)big;
  size_t len   = 0;
  size_t reser = CF_PRERESERVE;
  size_t slen  = strlen(small);

  while((pos = strstr(pos,small)) != NULL) {
    *pos = '\0';

    list[len++] = strdup(pre);

    if(len >= reser) {
      reser += CF_PRERESERVE;
      list = cf_alloc(list,reser,sizeof(*list),CF_ALLOC_REALLOC);
    }

    pre    = pos+slen;
    *pos++ = *small;
    if(len + 1 == max) break;
  }

  if(len + 1 <= max) {
    if(len >= reser) list = cf_alloc(list,++reser,sizeof(*list),CF_ALLOC_REALLOC);
    list[len++] = strdup(pre);
  }

  *ulist = list;

  return len;
}


/* eof */
