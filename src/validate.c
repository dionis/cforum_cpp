/**
 * \file validate.c
 * \author Christian Kruse, <cjk@wwwtech.de>
 * \brief Validation functions for several URI types
 *
 * This file contains validation functions for several
 * URI types (e.g. link and mail validation)
 *
 */

#include "config.h"
#include "defines.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <idna.h>

#include "validate.h"

static int is_valid_http_link_check(const char *link);

static const cf_scheme_list_t scheme_list[] = {
  { "http",     is_valid_http_link_check },
  { "https",    is_valid_http_link_check },
  { "mailto",   cf_is_valid_mailto_link },
  { "wais",     cf_is_valid_wais_link },
  { "prospero", cf_is_valid_prospero_link },
  { "telnet",   cf_is_valid_telnet_link },
  { "nntp",     cf_is_valid_nntp_link },
  { "news",     cf_is_valid_news_link },
  { "gopher",   cf_is_valid_gopher_link },
  { "ftp",      cf_is_valid_ftp_link },
  { "file",     cf_is_valid_file_link },
  { "\0", NULL }
};


static int isextra(char c) {
  switch(c) {
    case '!':
    case '*':
    case '\'':
    case '(':
    case ')':
    case ',':
      return 1;
  }

  return 0;
}

static int issafe(char c) {
  switch(c) {
    case '$':
    case '-':
    case '_':
    case '.':
    case '+':
      return 1;
  }

  return 0;
}

static int isuchar_wo_escape(char c) {
  /* escape is not checked! */
  if((!isalnum(c) || !isascii(c)) && !issafe(c) && !isextra(c)) return 0;
  return 1;
}

int cf_is_valid_hostname(const char *hostname) {
  register const char *ptr;
  char *out = NULL;

  if(*hostname == '-') return -1;

  /* Since we're not IDN-aware, let's change hostname to ASCII form */
  if(idna_to_ascii_8z(hostname,(char **)&out,IDNA_USE_STD3_ASCII_RULES) != IDNA_SUCCESS) return -1;

  for(ptr=out;*ptr;ptr++) {
    if((!isalnum(*ptr) && *ptr != '-') || !isascii(*ptr)) {
      if(*ptr == '.') {
        /*
         * two dots after another are not allowed, neither a dot at the beginning
         * A dash at the beginning or end of a label isn't allowed, too
         */
        if(ptr == out || *(ptr-1) == '.' || *(ptr-1) == '-' || *(ptr+1) == '-') {
          free(out);
          return -1;
        }

        continue;
      }

      free(out);
      return -1;
    }
  }

  /* ok, all characters are alnum and ascii or dots. Hostname is valid */
  free(out);
  return 0;
}

static int is_valid_http_link_check(const char *link) {
  /* we allow anchors by default */
  return cf_is_valid_http_link(link,0);
}

int cf_is_valid_http_link(const char *link,int strict) {
  register const char *ptr;
  const char *begin = NULL,*end = NULL;
  char *helper;

  /* first we check if the scheme is valid */
  if(strncmp(link,"http://",7)) {
    if(strncmp(link,"https://",8)) return -1;

    ptr = link+8;
  }
  else ptr = link + 7;

  begin = ptr;

  /* ok, it seems as if scheme is valid -- get hostname */
  for(;*ptr;ptr++) {
    if(*ptr == ':' || *ptr == '/' || *ptr == '#' || *ptr == '?') {
      end = ptr-1;
      break;
    }
  }

  /* URL consists only of a hostname if end is NULL */
  if(end == NULL) {
    end = ptr-1;
  }

  helper = strndup(begin,end-begin+1);
  if(cf_is_valid_hostname(helper) == -1) {
    /* ups, no valid hostname -- die */
    free(helper);
    return -1;
  }

  free(helper);

  /* hostname is valid; follows a port? */
  if(*ptr == ':') {
    /* port has to be digit+ */
    for(begin=++ptr;*ptr;ptr++) {
      /* port is valid (at least one digit followed by a slash) */
      if(*ptr == '/' && ptr > begin) break;

      /* hu? port must be digits... bad boy! */
      if(!isdigit(*ptr)) return -1;
    }
  }

  /* follows a host path? */
  if(*ptr == '/') {
    for(begin=ptr;*ptr;ptr++) {
      /* escape sequenz */
      if(*ptr == '%') {
        if(!isxdigit(*(ptr+1)) || !isxdigit(*(ptr+2))) return -1;
        ptr += 2;
        continue;
      }

      /* anchor means end of host path */
      if(*ptr == '#') break;

      if(!isuchar_wo_escape(*ptr)) {
        switch(*ptr) {
          case '/':
          case ';':
          case ':':
          case '@':
          case '&':
          case '=':
          case '~':
            continue;
          default:
            if(*ptr != '?') return -1;
        }

        /* could only be a question mark (end of host path) */
        break;
      }
    }
  }

  /* follows a search path? */
  if(*ptr == '?') {
    for(++ptr;*ptr;ptr++) {
      /* escaped character */
      if(*ptr == '%') {
        if(!isxdigit(*(ptr+1)) || !isxdigit(*(ptr+2))) return -1;
        ptr += 2;
        continue;
      }

      /* anchor means end of search path */
      if(*ptr == '#') break;

      /* we checked for escaped before */
      if(!isuchar_wo_escape(*ptr)) {
        switch(*ptr) {
          case ';':
          case ':':
          case '@':
          case '&':
          case '=':
          case '/':
            break;
          default:
            /* no anchor in strict mode */
            return -1;
        }
      }
    }
  }

  if(*ptr == '#') {
    if(strict) return -1;

    for(ptr++;*ptr;ptr++) {
      if(*ptr == '%') {
        if(!isxdigit(*(ptr+1)) || !isxdigit(*(ptr+2))) return -1;
        ptr += 2;
        continue;
      }

      if(!isuchar_wo_escape(*ptr)) {
        switch(*ptr) {
          case ';':
          case '/':
          case '?':
          case ':':
          case '@':
          case '&':
          case '=':
            break;
          default:
            return -1;
        }
      }
    }
  }

  return 0;
}

int cf_is_valid_mailto_link(const char *addr) {
  /* first, check if mail address begins with mailto: */
  if(strncmp(addr,"mailto:",7) == 0) {
    /* let is_valid_mailaddress() do the checks... */
    return cf_is_valid_mailaddress(addr+7);
  }

  /* not a mailto:-scheme -- bad guy! invalid! */
  return -1;
}

/*
 * Algorithm is in parts from 'Secure Cooking With C and C++'
 */
int cf_is_valid_mailaddress(const char *address) {
  register const char *c, *domain;
  static char *rfc822_specials = "()<>@,;:\\\"[]";

  /* first we validate the name portion (name@domain) */
  for(c=address;*c;c++) {
    if(*c == '\"' && (c == address || *(c - 1) == '.' || *(c - 1) == '\"')) {
      while (*++c) {
        if (*c == '\"') break;
        if (*c == '\\' && (*++c == ' ')) continue;
        if (*c <= ' ' || *c >= 127) return -1;
      }

      if(!*c++) return -1;
      if(*c == '@') break;
      if(*c != '.') return -1;
      continue;
    }

    if(*c == '@') break;
    if(*c <= ' ' || *c >= 127) return -1;
    if(strchr(rfc822_specials, *c)) return -1;
  }

  if(c == address || *(c - 1) == '.') return -1;

  /* next we validate the domain portion (name@domain) */
  if (!*(domain = ++c)) return -1;

  /*
   * we also have to accept Umlauts domains, which means we have
   * to check domain name more complicated than the secure cooking
   * does
   */
  return cf_is_valid_hostname(domain);
}

int cf_is_valid_wais_link(const char *link) {
  register const char *ptr;
  const char *begin = NULL,*end = NULL;
  char *helper;
  unsigned int slashes;

  /* first we check if the scheme is valid */
  if(strncmp(link,"wais://",7)) {
    return -1;
  }
  else ptr = (char *)link + 7;

  begin = ptr;

  /* ok, it seems as if scheme is valid -- get hostname */
  for(;*ptr;ptr++) {
    if(*ptr == ':' || *ptr == '/' || *ptr == '?') {
      end = ptr-1;
      break;
    }
  }

  /* URL consists only of a hostname if end is NULL */
  if(end == NULL) {
    end = ptr-1;
  }

  helper = strndup(begin,end-begin+1);
  if(cf_is_valid_hostname(helper) == -1) {
    /* ups, no valid hostname -- die */
    free(helper);
    return -1;
  }

  free(helper);

  /* hostname is valid; follows a port? */
  if(*ptr == ':') {
    /* port has to be digit+ */
    for(begin=++ptr;*ptr;ptr++) {
      /* port is valid (at least one digit followed by a slash) */
      if(*ptr == '/' && ptr > begin) break;

      /* hu? port must be digits... bad boy! */
      if(!isdigit(*ptr)) return -1;
    }
  }

  /* does not follow host path, no wais url*/
  if(*ptr++ != '/') return -1;

  slashes = 0;
  for(begin=ptr;*ptr;ptr++) {
    /* escape sequenz */
    if(*ptr == '%') {
      if(!isxdigit(*(ptr+1)) || !isxdigit(*(ptr+2))) return -1;
      ptr += 2;
      continue;
    }


    if(!isuchar_wo_escape(*ptr)) {
      switch(*ptr) {
        case '/':
          slashes++;
          if(slashes > 2) return -1;
          continue;
        default:
          if(*ptr != '?' || slashes) return -1;
      }
      /* could only be a question mark (end of host path) */
      break;
    }
  }

  /* three forms of wais url
     1. wais://hostport/database   (slashes == 0, !*ptr)
     2. wais://hostport/database/wtype/wpath (slashes == 2, !*ptr)
     3. wais://hostport/database?search (slashes == 0, *ptr == '?')
   */
  if(slashes && (slashes != 2 || *ptr)) return -1;

  /* follows a search path? */
  if(*ptr == '?') {
    for(++ptr;*ptr;ptr++) {
      /* escaped character */
      if(*ptr == '%') {
        if(!isxdigit(*(ptr+1)) || !isxdigit(*(ptr+2))) return -1;
        ptr += 2;
        continue;
      }

      /* we checked for escaped before */
      if(!isuchar_wo_escape(*ptr)) {
        switch(*ptr) {
          case ';':
          case ':':
          case '@':
          case '&':
          case '=':
            break;
          default:
            /* no anchor in strict mode */
            return -1;
        }
      }
    }
  }

  /* now url should definitely be at an end! */
  if (*ptr) return -1;

  return 0;

}

int cf_is_valid_prospero_link(const char *link) {
  register const char *ptr;
  const char *begin = NULL,*end = NULL;
  char *helper;
  int had_cur_equal = 0;

  /* first we check if the scheme is valid */
  if(strncmp(link,"prospero://",11)) {
    return -1;
  }
  else ptr = link + 11;

  begin = ptr;

  /* ok, it seems as if scheme is valid -- get hostname */
  for(;*ptr;ptr++) {
    if(*ptr == ':' || *ptr == '/') {
      end = ptr-1;
      break;
    }
  }

  /* URL consists only of a hostname if end is NULL */
  if(end == NULL) {
    end = ptr-1;
  }

  helper = strndup(begin,end-begin+1);
  if(cf_is_valid_hostname(helper) == -1) {
    /* ups, no valid hostname -- die */
    free(helper);
    return -1;
  }

  free(helper);

  /* hostname is valid; follows a port? */
  if(*ptr == ':') {
    /* port has to be digit+ */
    for(begin=++ptr;*ptr;ptr++) {
      /* port is valid (at least one digit followed by a slash) */
      if(*ptr == '/' && ptr > begin) break;

      /* hu? port must be digits... bad boy! */
      if(!isdigit(*ptr)) return -1;
    }
  }

  /* does not follow host path, no prospero url*/
  if(*ptr++ != '/') return -1;

  for(begin=ptr;*ptr;ptr++) {
    /* escape sequenz */
    if(*ptr == '%') {
      if(!isxdigit(*(ptr+1)) || !isxdigit(*(ptr+2))) return -1;
      ptr += 2;
      continue;
    }


    if(!isuchar_wo_escape(*ptr)) {
      switch(*ptr) {
        case '/':
        case '?':
        case ':':
        case '@':
        case '&':
        case '=':
          continue;
        default:
          if(*ptr != ';') return -1;
      }
      /* could only be a semicolon (end of host path) */
      break;
    }
  }

  /* follows a fieldspec? */
  if(*ptr == ';') {
    for(++ptr;*ptr;ptr++) {
      /* escaped character */
      if(*ptr == '%') {
        if(!isxdigit(*(ptr+1)) || !isxdigit(*(ptr+2))) return -1;
        ptr += 2;
        continue;
      }

      /* we checked for escaped before */
      if(!isuchar_wo_escape(*ptr)) {
        switch(*ptr) {
          case ';':
            if(!had_cur_equal) return -1;
            had_cur_equal = 0;
            break;
          case '=':
            if(had_cur_equal) return -1;
            had_cur_equal = 1;
            break;
          case '?':
          case ':':
          case '@':
          case '&':
            break;
          default:
            /* no anchor in strict mode */
            return -1;
        }
      }
    }
  }

  /* now url should definitely be at an end! */
  if (*ptr) return -1;

  return 0;
}

int cf_is_valid_telnet_link(const char *link) {
  register const char *ptr;
  const char *anchor,*doubledot;
  char *hostname,*mylink;
  size_t len = strlen(link);

  if(strncmp(link,"telnet://",9)) return -1;

  if(link[len-1] == '/') mylink = strndup(link,len-1);
  else mylink = (char *)link;

  if((anchor = strstr(mylink,"@")) == NULL) {
    if((doubledot = strstr(mylink+9,":")) == NULL) {
      hostname = strdup(mylink+9);
      if(cf_is_valid_hostname(hostname)) {
        if(mylink != link) free(mylink);
        free(hostname);
        return -1;
      }
      free(hostname);
    }
    else {
      hostname = strndup(mylink+9,doubledot-mylink-9);
      if(cf_is_valid_hostname(hostname)) {
        if(mylink != link) free(mylink);
        free(hostname);
        return -1;
      }
      free(hostname);

      for(ptr=doubledot+1;*ptr;ptr++) {
        if(!isdigit(*ptr)) {
          /* we do not allow anything else after the slash */
          if(*ptr == '/' && *(ptr+1)) {
            if(mylink != link) free(mylink);
            return -1;
          }
        }
      }
    }
  }
  else {
    if((doubledot = strstr(anchor+1,":")) == NULL) {
      if(cf_is_valid_hostname(anchor+1)) {
        if(mylink != link) free(mylink);
        return -1;
      }
    }
    else {
      hostname = strndup(anchor+1,doubledot-anchor-1);
      if(cf_is_valid_hostname(hostname)) {
        if(mylink != link) free(mylink);
        free(hostname);
        return -1;
      }
      free(hostname);

      for(ptr=doubledot+1;*ptr;ptr++) {
        if(!isdigit(*ptr)) {
          /* we do not allow anything else after the slash */
          if(*ptr == '/' && *(ptr+1)) {
            if(mylink != link) free(mylink);
            return -1;
          }
        }
      }
    }
  }

  /* ok then -- it seems as if hostname and port are all right, check username
   * and password, if exist
   */
  if(anchor) {
    for(ptr=mylink+9;*ptr;ptr++) {
      if(isuchar_wo_escape(*ptr) == 0) {
        /* end of username */
        if(*ptr == ':' || *ptr == '@') break;

        /* escaped char */
        if(*ptr == '%') {
          if(isxdigit(*(ptr+1)) == 0 || isxdigit(*(ptr+2)) == 0) {
            if(mylink != link) free(mylink);
            return -1;
          }
        }

        /* there are some special characters allowed */
        switch(*ptr) {
          case ';':
          case '?':
          case '&':
          case '=':
            break;
          default:
            if(mylink != link) free(mylink);
            return -1;
        }
      }
    }

    /* password       = *[ uchar | ";" | "?" | "&" | "=" ] */
    if(*ptr == ':') {
      for(++ptr;*ptr;++ptr) {
        if(isuchar_wo_escape(*ptr) == 0) {
          /* end of password */
          if(*ptr == '@') break;

          if(*ptr == '%') {
            if(isxdigit(*(ptr+1)) == 0 || isxdigit(*(ptr+2)) == 0) {
              if(mylink != link) free(mylink);
              return -1;
            }
          }

          switch(*ptr) {
            case ';':
            case '?':
            case '&':
            case '=':
              break;
            default:
              if(mylink != link) free(mylink);
              return -1;
          }
        }
      }
    }

    /* we have to be at the end of the username-password section now */
    if(*ptr != '@') {
      if(mylink != link) free(mylink);
      return -1;
    }
  }

  if(mylink != link) free(mylink);
  return 0;
}

int cf_is_valid_nntp_link(const char *link) {
  register const char *ptr;
  const char *end;
  char *hostname;

  if(strncmp(link,"nntp://",7)) return -1;
  else ptr = link + 7;

  if((end = strstr(ptr,"/")) == NULL) return -1;

  hostname = strndup(ptr,end-ptr);
  if(cf_is_valid_hostname(hostname)) {
    free(hostname);
    return -1;
  }
  free(hostname);

  ptr = end + 1;
  if(!isalpha(*ptr)) return -1;

  for(;*ptr;ptr++) {
    if(*ptr == '/') break;

    if(!isalnum(*ptr)) {
      switch(*ptr) {
        case '-':
        case '.':
        case '+':
        case '_':
          break;
        default:
          return -1;
      }
    }
  }

  if(*ptr == '/') {
    for(;*ptr;ptr++) {
      if(!isdigit(*ptr)) return -1;
    }
  }

  return 0;
}

int cf_is_valid_news_link(const char *link) {
  register const char *ptr;
  char *at,*doublept,*hostname;

  if(strncmp(link,"news:",5) != 0) return -1;

  ptr = link + 5;

  /* at least one character has to follow */
  if(*ptr == '\0') return -1;

  /* we accept news:* */
  if(strcmp(ptr,"*") == 0) return 0;


  /* we're in group mode */
  if((at = strstr(ptr,"@")) == NULL) {
    for(;*ptr;++ptr) {
      if(!isalnum(*ptr)) {
        switch(*ptr) {
          case '-':
          case '.':
          case '+':
          case '_':
            break;
          default:
            return -1;
        }
      }
    }
  }
  else {
    for(;*ptr;++ptr) {
      if(!isuchar_wo_escape(*ptr)) {
        if(*ptr == '@') break;

        if(*ptr == '%') {
          if(!isxdigit(*(ptr+1)) || !isxdigit(*(ptr+2))) return -1;
          ptr += 2;
          continue;
        }

        switch(*ptr) {
          case ';':
          case '/':
          case '?':
          case ':':
          case '&':
          case '=':
            break;
          default:
            return -1;
        }
      }
    }

    ++ptr;
    if((doublept = strstr(ptr,":")) == NULL) {
      if(cf_is_valid_hostname(ptr)) return -1;
    }
    else {
      hostname = strndup(ptr,doublept-ptr);
      if(cf_is_valid_hostname(hostname)) {
        free(hostname);
        return -1;
      }
      free(hostname);

      for(ptr=doublept+1;*ptr && isdigit(*ptr);++ptr);
      if(*ptr != '\0' && !isdigit(*ptr)) return -1;
    }
  }

  return 0;
}

int cf_is_valid_gopher_link(const char *link) {
  register const char *ptr;
  const char *begin = NULL,*end = NULL;
  char *helper;

  /* first we check if the scheme is valid */
  if(strncmp(link,"gopher://",9)) return -1;

  ptr = (char *)link + 9;

  begin = ptr;

  /* ok, it seems as if scheme is valid -- get hostname */
  for(;*ptr;ptr++) {
    if(*ptr == ':' || *ptr == '/') {
      end = ptr-1;
      break;
    }
  }

  /* URL consists only of a hostname if end is NULL */
  if(end == NULL) {
    end = ptr-1;
  }

  helper = strndup(begin,end-begin+1);
  if(cf_is_valid_hostname(helper) == -1) {
    /* ups, no valid hostname -- die */
    free(helper);
    return -1;
  }

  free(helper);

  /* hostname is valid; follows a port? */
  if(*ptr == ':') {
    /* port has to be digit+ */
    for(begin=++ptr;*ptr;ptr++) {
      /* port is valid (at least one digit followed by a slash) */
      if(*ptr == '/' && ptr > begin) break;

      /* hu? port must be digits... bad boy! */
      if(!isdigit(*ptr)) return -1;
    }
  }

  if(!*ptr) return 0;
  if(*ptr++ != '/') {
    return -1;
  }

  for(begin=ptr;*ptr;ptr++) {
    /* escape sequenz */
    if(*ptr == '%') {
      // follows search?
      if(*(ptr+1) == '0' && *(ptr+2) == '9') {
        ptr += 3;
        break;
      }
      if(!isxdigit(*(ptr+1)) || !isxdigit(*(ptr+2))) return -1;
      ptr += 2;
      continue;
    }

    if(!isuchar_wo_escape(*ptr)) {
      switch(*ptr) {
        case ';':
        case '/':
        case '?':
        case ':':
        case '@':
        case '&':
        case '=':
          continue;
        default:
          return -1;
      }
    }
  }

  // valid (search may be empty because it is defined as *[ ... ]!)
  if(!*ptr) return 0;

  /* follows a search path */
  for(;*ptr;ptr++) {
    /* escaped character */
    if(*ptr == '%') {
      // follows gopher+?
      if(*(ptr+1) == '0' && *(ptr+2) == '9') {
        ptr += 3;
        break;
      }
      if(!isxdigit(*(ptr+1)) || !isxdigit(*(ptr+2))) return -1;
      ptr += 2;
      continue;
    }

    /* we checked for escaped before */
    if(!isuchar_wo_escape(*ptr)) {
      switch(*ptr) {
        case ';':
        case ':':
        case '@':
        case '&':
        case '=':
          break;
        default:
          /* no anchor in strict mode */
          return -1;
      }
    }
  }

  // valid (gopher+ may be empty because it is defined as *xchar!)
  if(!*ptr) return 0;

  for(;*ptr;ptr++) {
    /* escaped character */
    if(*ptr == '%') {
      if(!isxdigit(*(ptr+1)) || !isxdigit(*(ptr+2))) return -1;
      ptr += 2;
      continue;
    }
    /* we checked for escaped before */
    if(!isuchar_wo_escape(*ptr)) {
      switch(*ptr) {
        case ';':
        case '/':
        case '?':
        case ':':
        case '@':
        case '&':
        case '=':
          break;
        default:
          /* no anchor in strict mode */
          return -1;
      }
    }
  }

  return 0;
}

int cf_is_valid_ftp_link(const char *link) {
  register const char *ptr = link;
  const char *at,*slash,*doublepoint,*start;
  char *hostname;
  int dd = 0;

  if(strncmp(link,"ftp://",6)) return -1;

  /*
   * it doesnt matter if we only have a username or a password
   * and a username. Both consist of *[uchar|";"|"?"|"&"|"="]
   */
  if((at = strstr(link+6,"@")) != NULL) {
    for(ptr=link+6;ptr<at;ptr++) {
      if(!isuchar_wo_escape(*ptr)) {
        if(*ptr == '%') {
          if(!isxdigit(*(ptr+1)) || !isxdigit(*(ptr+2))) return -1;
          ptr += 2;
          continue;
        }

        switch(*ptr) {
          case ';':
          case '?':
          case '&':
          case '=':
            break;
          case ':':
            if(dd == 0) {
              dd = 1;
              break;
            }
          default:
            return -1;
        }
      }
    }
  }

  start = at ? at + 1 : link + 6;

  slash       = strstr(start,"/");
  doublepoint = strstr(start,":");

  if(!slash) {
    if(!doublepoint) {
      if(cf_is_valid_hostname(start)) return -1;
    }
    else {
      hostname = strndup(start,doublepoint-start);
      if(cf_is_valid_hostname(hostname)) {
        free(hostname);
        return -1;
      }
      free(hostname);

      for(ptr=doublepoint+1;*ptr;ptr++) {
        if(!isdigit(*ptr)) return -1;
      }
    }
  }
  /* a slash exists */
  else {
    if(doublepoint) {
      hostname = strndup(start,doublepoint-start);
      if(cf_is_valid_hostname(hostname)) {
        free(hostname);
        return -1;
      }
      free(hostname);

      for(ptr=doublepoint+1;*ptr != '/';ptr++) {
        if(!isdigit(*ptr)) return -1;
      }
    }
    else {
      hostname = strndup(start,slash-start);
      if(cf_is_valid_hostname(hostname)) {
        free(hostname);
        return -1;
      }
      free(hostname);
    }
  }

  if(slash) {
    for(ptr=slash+1;*ptr;ptr++) {
      if(!isuchar_wo_escape(*ptr)) {
        if(*ptr == '%') {
          if(!isxdigit(*(ptr+1)) || !isxdigit(*(ptr+2))) return -1;
          ptr += 2;
          continue;
        }

        if(*ptr == ';') {
          break;
        }

        switch(*ptr) {
          case '?':
          case ':':
          case '@':
          case '&':
          case '=':
          case '/':
            break;
          default:
            return -1;
        }
      }
    }
  }

  if(*ptr == ';') {
    switch(*(ptr+1)) {
      case 'A':
      case 'I':
      case 'D':
      case 'a':
      case 'i':
      case 'd':
        break;
      default:
        return -1;
    }

    if(*(ptr+2)) return -1;
  }

  return 0;
}

int cf_is_valid_file_link(const char *link) {
  register char *ptr = NULL;
  const char *hostname;
  char *dp,*slash;

  if(strncmp(link,"file://",7)) return -1;
  if(strncmp(link+7,"localhost",9) == 0) {
    if(*(link+16) != '/') return -1;
    slash = (char *)link+16;
  }
  else {
    /* ok, no file path follows; this is not allowed */
    if((slash = strstr(link+7,"/")) == NULL) return -1;

    /* we got a hostname without port number */
    if((dp = strstr(link+7,":")) == NULL) {
      hostname = strndup(link+7,slash-link+7);
      if(!cf_is_valid_hostname(hostname)) {
        free((void *)hostname);
        return -1;
      }
      free((void *)hostname);
    }
    /* we got a hostname with port number */
    else {
      hostname = strndup(link+7,dp-link+7);
      if(!cf_is_valid_hostname(hostname)) {
        free((void *)hostname);
        return -1;
      }
      free((void *)hostname);

      for(ptr=dp+1;*ptr;ptr++) {
        if(!isdigit(*ptr)) return -1;
      }
    }
  }

  /* ok, now fpath segments follow */
  for(ptr=slash+1;*ptr;++ptr) {
    if(!isuchar_wo_escape(*ptr)) {
      if(*ptr == '%') {
        if(!isxdigit(*(ptr+1)) || !isxdigit(*(ptr+2))) return -1;
        ptr += 2;
        continue;
      }

      if(*ptr == ';') break;

      switch(*ptr) {
        case '?':
        case ':':
        case '@':
        case '&':
        case '=':
        case '/':
          break;
        default:
          return -1;
      }
    }
  }

  return 0;
}

/**
 * Wrapper function; it checks for a scheme and calls the
 * scheme validation function
 */
int cf_is_valid_link(const char *link) {
  char *ptr = strstr(link,"://");
  char scheme[20];
  int i,vs = 0;

  /*
   * no scheme found, but mailto-links are mailto:<address> and news-links are
   * news:<address>, not mailto://<address> and not news://<address> -- so we
   * have to do an extra check
   */
  if(ptr == NULL) {
    if(strncmp(link,"mailto:",7) == 0)    strcpy(scheme,"mailto");
    else if(strncmp(link,"news:",5) == 0) strcpy(scheme,"news");
    else return -1;
  }
  else {
    if(strncmp(link,"view-source:",12) == 0) {
      link += 12;
      vs = 1;
    }

    if(ptr-link >= 20) return -1; /* hu, seems not to be a valid scheme */

    strncpy(scheme,link,ptr-link);
    scheme[ptr-link] = '\0';

    /* view-source may only be used with http and https */
    if(vs && strcmp(scheme,"http") != 0 && strcmp(scheme,"https") != 0) return -1;
  }

  if(vs) return is_valid_http_link_check(link);
  else {
    for(i=0;scheme_list[i].validator;i++) {
      if(strcmp(scheme_list[i].scheme,scheme) == 0) {
        return scheme_list[i].validator(link);
      }
    }
  }

  return -1;
}


#ifdef CF_VALIDATE_AS_PROGRAM
int main(int argc,char *argv[]) {
  int i;

  if(argc == 1) {
    fprintf(stderr,"Usage:\n\t%s uris\n",argv[0]);
    return EXIT_FAILURE;
  }

  for(i=1;i<argc;i++) {
    if(is_valid_link(argv[i]) == -1) {
      printf("%s is not a valid URI\n",argv[i]);
    }
    else {
      printf("%s is a valid URI\n",argv[i]);
    }

    return EXIT_SUCCESS;
  }

  return EXIT_SUCCESS;
}
#endif

/* eof */
