/**
 * \file readline.h
 * \author Christian Kruse, <cjk@wwwtech.de>
 * \brief the readline definitions
 *
 * This file includes the definitions for a thread-safe, buffered readline algorithm. It was originally
 * developed by Richard Stevens, but I did some small changes.
 */

#ifndef CF_READLINE_H
#define CF_READLINE_H

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

/*!
 * This structure contains the necessary information about the read data
 */
typedef struct cf_rline_s {
  int rl_cnt; /*!< The number of characters read into rl_buf */
  int rl_mem; /*!< The number of bytes reserved for the return value of readline(). Set in every call of readline() */
  int rl_len; /*!< The length of the string returned by readline(). Set in every call of readline() */
  char *rl_bufptr; /*!< The pointer to the position in rl_buf. */
  char rl_buf[CF_BUFSIZ]; /*!< The buffer; in this buffer will be saved read data. */
} cf_rline_t;

/*!
 * This function reads a complete line from a socket and buffers the data which was not needed within this call.
 * \param fd The file descriptor to read from
 * \param tsd The buffer structure used to buffer
 * \return The line as an char * string on success, NULL on failure.
 * \attention You have to free() the returned string!
 */
char *cf_readline(int fd,cf_rline_t *tsd);

/*!
 * This function tries to write n bytes to a file descriptor. It re-tries it on EAGAIN and EINTR.
 * If not all of the data could be written in the first call, it makes more calls to write the rest of the
 * data.
 * \param fd   The filedescriptor to write to.
 * \param vptr The pointer to the data to write
 * \param n    The size of the data to write
 * \return The number of bytes written.
 */
ssize_t cf_writen(int fd,const void *vptr,size_t n);

#endif

/* eof */
