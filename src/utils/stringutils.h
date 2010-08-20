/*!
 * \file stringutils.c
 * \author Christian Kruse, <cjk@wwwtech.de>
 * \brief string utilities for the Classic Forum
 *
 * A string abstraction for the Classic Forum
 */

#ifndef CF_STRING_UTILS_H
#define CF_STRING_UTILS_H

#include "config.h"
#include "defines.h"

#include <string.h>
#include <stdlib.h>

#include <unicode/ustring.h>

#include "memoryutils.h"

/*!
 * This is the heart of the string abstraction. It contains the string itself, the length
 * of the string and the size of the reserved memory for the string.
 */
typedef struct s_string {
  int32_t len; /*!< length of the memory used */
  int32_t reserved; /*!< length of the memory area itself */
  int growth; /*!< Defining by which size the string memory should grow */
  UChar *content; /*!< The memory area itself */
} cf_string_t;

#define CF_STRING_INITIALIZER { 0, 0, CF_BUFSIZ, NULL }

/*!
 * This function initializes a string structure with specified growth factor.
 * \param str A reference to a string structure
 * \param growth The growth factor
 */
void cf_str_init_growth(cf_string_t *str,int growth);

#define cf_str_init(str) cf_str_init_growth((str),CF_BUFSIZ)

/*!
 * This function frees the reserved memory in a string structure and sets everything to NULL
 * \param str A reference to the string structure
 */
void cf_str_cleanup(cf_string_t *str);

/*!
 * This function appends a character to the string
 * \param str A reference to the string structure
 * \param content The character to append
 * \return Number of characters appended on success or 0 on failure
 */
int32_t cf_str_char_append(cf_string_t *str,const UChar content);

/*!
 * This function appends a char array to the string in the string structure.
 * \param str A reference to the string structure
 * \param content The char array to append
 * \param length The length of the char array
 * \return The number of characters appended on success or 0 on failure
 */
int32_t cf_str_chars_append(cf_string_t *str,const UChar *content,const int32_t length);

/*!
 * This function appends a code point in utf16 representation to the string in the string structure
 * \param str A reference to the string structure
 * \param content The code point to append
 * \return The number of bytes appended on success or 0 on failure
 */
int32_t cf_str_uchar32_append(cf_string_t *str,const UChar32 content);

/*!
 * This function appends a string structure to a string structure. It's just a wrapper for
 * the cf_str_chars_append() function.
 * \param str A reference to the string structure to append to
 * \param content A reference to the string structure to append
 */
int32_t cf_str_str_append(cf_string_t *str,const cf_string_t *content);

/*!
 * This function appends a C-like null terminated character array to a string structure.
 * It's just a wrapper for the cf_str_chars_append() function.
 * \param str A reference to the string structure to append to
 * \param content The char array to append
 * \return The number of characters appended on success or 0 on failure
 */
int32_t cf_str_cstr_append(cf_string_t *str,const UChar *content);

/*!
 * This function sets the value of an string structure to a given char array. The old string contained
 * in the structure will be lost. It gets the length of the char array by strlen().
 * \param str A reference to the string to append to
 * \param content The string to set
 * \return The number of characters set on success or 0 on failure
 */
int32_t cf_str_cstr_set(cf_string_t *str,const UChar *content);

/*!
 * This function sets the value of an string structure to a given char array. The old string contained
 * in the structure will be lost.
 * \param str A reference to the string to append to
 * \param content The string to set
 * \param length The length of the string to set
 * \return The number of characters set on success or 0 on failure
 */
int32_t cf_str_char_set(cf_string_t *str,const UChar *content,const int32_t length);

/*!
 * This function sets the value of a string structure to the value of another string structure. The old
 * string contained in the target structure will be lost.
 * \param str A reference to the structure to set to
 * \param content A reference to the structure to set
 * \return The number of characters set on success or 0 on failure
 */
int32_t cf_str_str_set(cf_string_t *str,const cf_string_t *content);

/*!
 * This function tests if two strings (cf_string_t) are equal
 * \param str1 string 1
 * \param str2 string 2
 * \return TRUE if both equal, FALSE otherwise
 */
int cf_str_eq_string(const cf_string_t *str1,const cf_string_t *str2);

/*!
 * This function tests if two strings (cf_string_t, char *) are equal
 * \param str1 string 1
 * \param str2 string 2
 * \param len Length of string str2
 * \return TRUE if both equal, FALSE otherwise
 */
int cf_str_eq_chars(const cf_string_t *str1,const UChar *str2, const int32_t len);

/*!
 * Copies a UChar* string
 * \param src The source string
 * \param len The length of the string; can be -1, in this case u_strlen() gets called to determine the string length
 * \return NULL on failure, the copied string on success
 * \attention The caller has to free() the string!
 */
UChar *cf_strdup(const UChar *src,int32_t len);

/*!
 * This function converts an UTF-8 char * to an UTF-16 UChar *
 * \param src The source string
 * \param len The length of the source string; may be -1, in this case the string has to be 0 terminated
 * \param destlen The destination length will be stored in it if != NULL. If it is NULL, it will be ignored.
 * \return The converted string
 */
UChar *cf_to_utf16(const char *src,int32_t len,int32_t *destlen);

/*!
 * This function converts an UTF-16 UChar * to an UTF-8 char *
 * \param src The source string
 * \param len The length of the source string; may be -1, in this case the string has to be 0 terminated
 * \param destlen The destination length will be stored in it if != NULL. If it is NULL, it will be ignored.
 * \return The converted string
 */
char *cf_to_utf8(const UChar *src,int32_t len,int32_t *destlen);

#ifndef HAS_STRNDUP
/*!
 * Duplicate up to \c len characters in \c src
 * \param src The source string
 * \param len The max length of the strings copied
 * \return Returns the new string (caller has to free() it!)
 */
char *strndup(const char *src,size_t len);
#endif

#endif

/* eof */
