/**
 * \file datautils.c
 * \author Christian Kruse, <cjk@wwwtech.de>
 * \brief Data type utilities for the Classic Forum
 *
 * Data type utilities such as int64 types to string and
 * vice versa
 */

#include "datautils.h"

u_int64_t cf_uchr_to_uint64(const UChar *str,const int32_t len,int *errcode) {
  u_int64_t retval = 0;
  int32_t i = 0;
  UChar32 c;

  if(len < 0 || str == NULL) {
    *errcode = CF_ERR_ILPARAM;
    return 0;
  }

  while(i<len) {
    U16_NEXT(str,i,len,c);
    if(!u_isdigit(c)) break;

    retval = retval * 10 + u_digit(c,10);
  }

  *errcode = 0;
  return retval;
}

int64_t cf_uchr_to_int64(const UChar *str,const int32_t len,int *errcode) {
  int64_t retval = 0;
  int32_t i = 0;
  UChar32 c;
  int neg = 0;

  if(len < 0 || str == NULL) {
    *errcode = CF_ERR_ILPARAM;
    return 0;
  }

  while(i<len) {
    U16_NEXT(str,i,len,c);
    if(c == '-') { /* this works because code point for '-' is the same as the usascii value for '-' */
      neg = 1;
      continue;
    }

    if(!u_isdigit(c)) break;
    retval = retval * 10 + u_digit(c,10);
  }

  *errcode = 0;
  if(neg) retval = retval * -1;
  return retval;
}

int cf_uint64_to_str(cf_string_t *str, u_int64_t num) {
  UChar buff[50]; /* a number of uint64_t cannot have more than 20 digits, so 50 is totally safe */
  UChar32 c;
  int32_t len = 0,digit;
  UBool is_err = FALSE;

  if(str == NULL) return CF_ERR_ILPARAM;

  if(num) {
    while(num) {
      digit = num % 10;
      c = u_forDigit(digit,10);
      U16_APPEND(buff,len,50,c,is_err);
      num /= 10;

      if(is_err) return CF_ERR_UNICODE;
    }

    /* swap it */
    while(len > 0) {
      U16_PREV(buff,0,len,c)
      cf_str_uchar32_append(str,c);
    }

    cf_str_chars_append(str,buff,len);
  }
  else cf_str_uchar32_append(str,(UChar32)'0');

  return 0;
}

int cf_int64_to_str(cf_string_t *str, int64_t num) {
  UChar buff[50]; /* a number of int64_t cannot have more than 10 digits, so 50 is totally safe */
  UChar32 c;
  int32_t len = 0,digit;
  UBool is_err = FALSE;
  int neg = 0;

  if(str == NULL) return CF_ERR_ILPARAM;

  if(num) {
    if(num < 0) {
      neg = 1;
      num = num * -1;
    }

    while(num) {
      digit = num % 10;
      c = u_forDigit(digit,10);
      U16_APPEND(buff,len,50,c,is_err);
      num /= 10;

      if(is_err) return CF_ERR_UNICODE;
    }

    /* this works because the code point for '-' has the same value as the usascii code for '-' */
    if(neg) cf_str_uchar32_append(str,(UChar32)'-');

    /* swap it */
    while(len > 0) {
      U16_PREV(buff,0,len,c)
      cf_str_uchar32_append(str,c);
    }

    cf_str_chars_append(str,buff,len);
  }
  else cf_str_uchar32_append(str,(UChar32)'0');

  return 0;
}

/* eof */
