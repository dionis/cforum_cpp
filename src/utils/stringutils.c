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

  if(str->len + sizeof(*str->content) >= str->reserved) {
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

  if(str->len + (U16_LENGTH(content) * sizeof(*str->content)) >= str->reserved) {
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
  size_t len;

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

/* eof */
