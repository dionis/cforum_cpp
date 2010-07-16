/*!
 * \file utils.c
 * \brief Utilities for the Classic Forum, making the life easier
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * Utilities for the Classic Forum, such as getline(), make_path()
 * remove_path(), etc, pp making the life easier
 */

#ifndef CF_UTILS_H
#define CF_UTILS_H

/*!
 * UTF8 version of strcmp(): compare two strings
 * \param str1 First string
 * \param str2 Second string
 * \return 0 if strings are equal, -1 if strings are not equal
 */
int cf_strcasecmp(const char *str1,const char *str2);

/*!
 * UTF8 version of strncasecmp(): compare \c n bytes of two strings
 * \param str1 First string
 * \param str2 Second string
 * \param n The number of bytes to compare
 * \return 0 if strings are equal, -1 if strings are not equal
 */
int cf_strncasecmp(const char *str1,const char *str2,size_t n);

/*!
 * Count characters of an utf8 string
 * \param _s The string to count
 * \return The number of characters found (not the number of bytes!)
 * \author Colin Percival, <http://www.daemonology.net/blog/2008-06-05-faster-utf8-strlen.html>
 */
size_t cf_strlen_utf8(const char * _s);

/*!
 * Count bytes of a string. We use our own implementation because the glibc's sucks
 * \param str The string to count
 * \return The number of bytes found
 */
size_t cf_strlen(const char *str);

/*!
 * Remove a directory with its contents (recursively) or a file
 * \param path The directory/file to remove
 * \return 0 if directory/file could be removed successfully, -1 if an error occured
 * \attention This operation is \b NOT atomic!
 */
int cf_remove_path(const char *path);

/*!
 * Create a directory with subdirectories, e.g. cf_make_path("/tmp/test/test")
 * \param path The directories to create
 * \param mode The mode
 * \return 0 if directory/file could be removed successfully, 1 if an error occured
 * \attention This operation is \b NOT atomic!
 */
int cf_make_path(const char *path,mode_t mode);

#ifdef HAS_NO_GETLINE
/*!
 * Read a complete line from a file
 * \param lineptr A reference to a char * pointer, line will be stored in it
 * \param n A reference to a size_t, length will be stored in it
 * \param stream The FILE* stream to read from
 * \return The number of bytes read or -1 on failure
 */
size_t getline(char **lineptr,size_t *n,FILE *stream);
#endif

#ifdef HAS_NO_GETDELIM
/*!
 * This function reads from a file until 'delim' has been found
 * \param lineptr A reference to a char * pointer, line will be stored in it
 * \param n A reference to a size_t, length will be stored in it
 * \param delim The delimiter character
 * \param stream The FILE* stream to read from
 * \return The number of bytes read or -1 on failure
 */
size_t getdelim(char **lineptr,size_t *n,int delim,FILE *stream);
#endif

#ifdef NOSTRDUP
/*!
 * This function duplicates a string, allocating memory on heap
 * \param str The string to duplicate
 * \return The new string or NULL on failure
 * \attention YOU have to \c free() the string again!
 */
u_char *strdup(const char *str);
#endif

#ifdef NOSTRNDUP
/*!
 * This function duplicates \c len bytes of a string, allocating memory on heap
 * \param str The string to duplicate
 * \return The new string or NULL on failure
 * \attention YOU have to \c free() the string again!
 */
u_char *strndup(const char *str,size_t len);
#endif


/**
 * Splits a string into a list of strings and returns the length of the list. The string big will be cut at
 * every match of the string \c small
 * \param big The string to split
 * \param small The string to search
 * \param ulist A reference to a char ** pointer. In this char ** pointer will the list be stored.
 * \return The length of the list
 * \attention YOU have to free every element of the list and the list itself!
 */
size_t split(const char *big,const char *small,char ***ulist);

/**
 * Splits a string into a list of maximal max elements
 * \param big The string to split
 * \param small The string to search
 * \param ulist A reference to a u_char ** pointer
 * \param max The count of maximum splits
 * \return The length of the list
 * \attention YOU have to free every element of the list _and_ the list itself!
 */
size_t nsplit(const char *big,const char *small,char ***ulist,size_t max);

# endif

/* eof */