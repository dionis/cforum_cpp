/*!
 * \file utils.c
 * \brief Utilities for the Classic Forum, making the life easier
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * Utilities for the Classic Forum, such as getline(), make_path()
 * remove_path(), etc, pp making the life easier
 */

#ifndef _CF_UTILS_H_
#define _CF_UTILS_H_

/*!
 * \brief UTF8 version of strcmp(): compare two strings
 * \arg \c str1 First string
 * \arg \c str2 Second string
 * \return 0 if strings are equal, -1 if strings are not equal
 */
int cf_strcasecmp(const u_char *str1,const u_char *str2);

/*!
 * \brief UTF8 version of strncasecmp(): compare \c n bytes of two strings
 * \arg \c str1 First string
 * \arg \c str2 Second string
 * \arg \c n The number of bytes to compare
 * \return 0 if strings are equal, -1 if strings are not equal
 */
int cf_strncasecmp(const u_char *str1,const u_char *str2,size_t n);

/*!
 * \brief Count characters of an utf8 string
 * \arg \c _s The string to count
 * \return The number of characters found (not the number of bytes!)
 * \author Colin Percival, <http://www.daemonology.net/blog/2008-06-05-faster-utf8-strlen.html>
 */
size_t cf_strlen_utf8(const char * _s);

/*!
 * \brief Count bytes of a string. We use our own implementation because the glibc's sucks
 * \arg \c s The string to count
 * \return The number of bytes found
 */
size_t cf_strlen(register const char *s);

/*!
 * \brief Remove a directory with its contents (recursively) or a file
 * \arg \c path The directory/file to remove
 * \return 0 if directory/file could be removed successfully, -1 if an error occured
 * \attention This operation is \b NOT atomic!
 */
int cf_remove_path(const u_char *path);

/*!
 * \brief Create a directory with subdirectories, e.g. cf_make_path("/tmp/test/test")
 * \arg \c path The directories to create
 * \arg \c mode The mode
 * \return 0 if directory/file could be removed successfully, 1 if an error occured
 * \attention This operation is \b NOT atomic!
 */
int cf_make_path(const u_char *path,mode_t mode);

#ifdef HAS_NO_GETLINE
/*!
 * \brief Read a complete line from a file
 * \arg \c lineptr A reference to a char * pointer, line will be stored in it
 * \arg \c n A reference to a size_t, length will be stored in it
 * \arg \c stream The FILE* stream to read from
 * \return The number of bytes read or -1 on failure
 */
size_t getline(char **lineptr,size_t *n,FILE *stream);
#endif

#ifdef HAS_NO_GETDELIM
/*!
 * \brief This function reads from a file until 'delim' has been found
 * \arg \c lineptr A reference to a char * pointer, line will be stored in it
 * \arg \c n A reference to a size_t, length will be stored in it
 * \arg \c delim The delimiter character
 * \arg \c stream The FILE* stream to read from
 * \return The number of bytes read or -1 on failure
 */
size_t getdelim(char **lineptr,size_t *n,int delim,FILE *stream);
#endif

# endif

/* eof */