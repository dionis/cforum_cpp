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

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

#include <dirent.h>

#include "memoryutils.h"

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

#ifndef HAVE_GETLINE
/*!
 * Read a complete line from a file
 * \param lineptr A reference to a char * pointer, line will be stored in it
 * \param n A reference to a size_t, length will be stored in it
 * \param stream The FILE* stream to read from
 * \return The number of bytes read or -1 on failure
 */
size_t getline(char **lineptr,size_t *n,FILE *stream);
#endif

#ifndef HAVE_GETDELIM
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
