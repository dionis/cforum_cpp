/*!
 * \file stringutils.c
 * \author Christian Kruse, <cjk@wwwtech.de>
 * \brief string utilities for the Classic Forum
 *
 * A string abstraction for the Classic Forum
 */

#include "stringutils.h"

void cf_str_init_growth(cf_string_t *str,unsigned growth) {
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

size_t cf_str_char_append(cf_string_t *str,const char content) {
  if(str->growth == 0) str->growth = CF_BUFSIZ;

  if(str->len + 1 >= str->reserved) {
    str->reserved += str->growth;
    str->content   = cf_alloc(str->content,(size_t)str->reserved,1,CF_ALLOC_REALLOC);
  }

  str->content[str->len] = content;
  str->len              += 1;
  str->content[str->len] = '\0';

  return 1;
}

size_t cf_str_chars_append(cf_string_t *str,const char *content,size_t length) {
  size_t len;

  if(str->growth == 0) str->growth = CF_BUFSIZ;
  len = str->growth;

  if(str->len + length >= str->reserved) {
    if(length >= len) len += length;

    str->reserved += len;
    str->content   = cf_alloc(str->content,(size_t)str->reserved,1,CF_ALLOC_REALLOC);
  }

  memcpy(&str->content[str->len],content,length);
  str->len += length;
  str->content[str->len] = '\0';

  return length;
}

int cf_str_equal_string(const cf_string_t *str1,const cf_string_t *str2) {
  register char *ptr1 = str1->content,*ptr2 = str2->content;
  register size_t i;

  if(str1->len != str2->len) return 1;

  for(i = 0; i < str1->len; ++i,++ptr1,++ptr2) {
    if(*ptr1 != *ptr2) return 1;
  }

  return 0;
}

int cf_str_equal_chars(const cf_string_t *str1,const char *str2, size_t len) {
  register size_t i = 0;
  register char *ptr1 = str1->content,*ptr2 = (char *)str2;

  if(str1->len != len) return 1;

  for(i = 0; i < len; ++i,++ptr1,++ptr2) {
    if(*ptr1 != *ptr2) return 1;
  }

  return 0;
}

size_t cf_str_str_append(cf_string_t *str,cf_string_t *content) {
  return cf_str_chars_append(str,content->content,content->len);
}

size_t cf_str_cstr_append(cf_string_t *str,const char *content) {
  return cf_str_chars_append(str,content,strlen(content));
}

int cf_str_cstr_set(cf_string_t *str,const char *content) {
  return cf_str_char_set(str,content,strlen(content));
}

size_t cf_str_char_set(cf_string_t *str,const char *content,size_t length) {
  size_t len;

  if(str->growth == 0) str->growth = CF_BUFSIZ;
  len = str->growth;

  if(str->len + length >= str->reserved) {
    if(length >= len) len += length;

    str->reserved  = len;
    str->content   = cf_alloc(str->content,len,1,CF_ALLOC_REALLOC);
  }

  memcpy(str->content,content,length);
  str->len = length;
  str->content[length] = '\0';

  return length;
}

size_t cf_str_str_set(cf_string_t *str,cf_string_t *set) {
  return cf_str_char_set(str,set->content,set->len);
}



/* eof */
