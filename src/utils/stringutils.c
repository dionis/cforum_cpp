/*!
 * \file stringutils.c
 * \author Christian Kruse, <cjk@wwwtech.de>
 * \brief string utilities for the Classic Forum
 *
 * A string abstraction for the Classic Forum
 */

#include "stringutils.h"

void cf_str_init_growth(cf_string_t *str,int growth) {
  str->len      = 0;
  str->reserved = 0;
  str->growth   = growth;
  str->content  = NULL;
}

void cf_str_cleanup(cf_string_t *str) {
  str->len      = 0;
  str->reserved = 0;

  if(str->content) free(str->content);
  str->content  = NULL;
}

int32_t cf_str_char_append(cf_string_t *str,const UChar content) {
  if(str->growth == 0) str->growth = CF_BUFSIZ;

  if(str->len + (int32_t)sizeof(*str->content) >= str->reserved) {
    str->reserved += str->growth;
    str->content   = cf_alloc(str->content,(size_t)str->reserved,sizeof(*str->content),CF_ALLOC_REALLOC);
  }

  str->content[str->len] = content;
  str->len              += 1;
  str->content[str->len] = 0;

  return 1;
}

int32_t cf_str_uchar32_append(cf_string_t *str,const UChar32 content) {
  UBool is_err = FALSE;

  if(str->growth == 0) str->growth = CF_BUFSIZ;

  if(str->len + (U16_LENGTH(content) * (int32_t)sizeof(*str->content)) >= str->reserved) {
    str->reserved += str->growth;
    str->content   = cf_alloc(str->content,(size_t)str->reserved,sizeof(*str->content),CF_ALLOC_REALLOC);
  }

  U16_APPEND(str->content,str->len,str->reserved,content,is_err);
  str->content[str->len] = 0;

  return U16_LENGTH(content);
}

int32_t cf_str_chars_append(cf_string_t *str,const UChar *content,const int32_t length) {
  int32_t len;

  if(str->growth == 0) str->growth = CF_BUFSIZ;
  len = str->growth;

  if(str->len + length >= str->reserved) {
    if(length >= len) len += length;

    str->reserved += len;
    str->content   = cf_alloc(str->content,(size_t)str->reserved,sizeof(*str->content),CF_ALLOC_REALLOC);
  }

  memcpy(&str->content[str->len],content,length);
  str->len += length;
  str->content[str->len] = 0;

  return length;
}

int cf_str_eq_string(const cf_string_t *str1,const cf_string_t *str2) {
  return u_strcmp(str1->content,str2->content);
}

int cf_str_eq_chars(const cf_string_t *str1,const UChar *str2, const int32_t len) {
  return u_strncmp(str1->content,str2,len);
}

int32_t cf_str_str_append(cf_string_t *str,const cf_string_t *content) {
  return cf_str_chars_append(str,content->content,content->len);
}

int32_t cf_str_cstr_append(cf_string_t *str,const UChar *content) {
  return cf_str_chars_append(str,content,u_strlen(content));
}

int32_t cf_str_cstr_set(cf_string_t *str,const UChar *content) {
  return cf_str_char_set(str,content,u_strlen(content));
}

int32_t cf_str_char_set(cf_string_t *str,const UChar *content,const int32_t length) {
  int32_t len;

  if(str->growth == 0) str->growth = CF_BUFSIZ;
  len = str->growth;

  if(str->len + length >= str->reserved) {
    if(length >= len) len += length;

    str->reserved  = len;
    str->content   = cf_alloc(str->content,len,sizeof(*str->content),CF_ALLOC_REALLOC);
  }

  memcpy(str->content,content,length);
  str->len = length;
  str->content[length] = 0;

  return length;
}

int32_t cf_str_str_set(cf_string_t *str,const cf_string_t *set) {
  return cf_str_char_set(str,set->content,set->len);
}

UChar *cf_strdup(const UChar *src,int32_t len) {
  UChar *out;

  if(len == 0 || len < -1 || src == NULL) return NULL;
  if(len == -1) len = u_strlen(src);

  out = cf_alloc(NULL,len+1,sizeof(*out),CF_ALLOC_MALLOC);
  u_strncpy(out,src,len);
  out[len] = 0;

  return out;
}

UChar *cf_to_utf16(const char *src,int32_t len,int32_t *destlen) {
  UChar *dest = NULL;
  int32_t dlen = 0;
  UErrorCode err = 0;

  if(src == NULL || len < -1 || len == 0) return NULL;
  if(len == -1) len = strlen(src);

  u_strFromUTF8(NULL,0,&dlen,src,len,&err);
  if(dlen <= 0) return NULL;

  err = 0;
  dest = cf_alloc(NULL,sizeof(*dest),dlen+1,CF_ALLOC_MALLOC);
  u_strFromUTF8(dest,dlen+1,&dlen,src,len,&err);

  if(U_FAILURE(err)) {
    free(dest);
    return NULL;
  }

  if(destlen) *destlen = dlen;
  return dest;
}

char *cf_to_utf8(const UChar *src,int32_t len,int32_t *destlen) {
  char *dest = NULL;
  int32_t dlen = 0;
  UErrorCode err = 0;

  if(src == NULL || len < -1 || len == 0) return NULL;
  if(len == -1) len = u_strlen(src);

  u_strToUTF8(NULL,0,&dlen,src,len,&err);
  if(dlen <= 0) return NULL;

  err = 0;
  dest = cf_alloc(NULL,sizeof(*dest),dlen+1,CF_ALLOC_MALLOC);
  u_strToUTF8(dest,dlen+1,&dlen,src,len,&err);

  if(U_FAILURE(err)) {
    free(dest);
    return NULL;
  }

  if(destlen) *destlen = dlen;
  return dest;
}

#ifndef HAVE_STRNDUP
char *strndup(const char *src,size_t len) {
  char *dst = cf_alloc(NULL,sizeof(*dst),len+1,CF_ALLOC_MALLOC);
  memcpy(dst,src,len);
  dst[len] = 0;

  return dst;
}
#endif

/* eof */
