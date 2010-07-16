/**
 * \file readline.c
 * \author Christian Kruse, <cjk@wwwtech.de>
 * \brief The readline implementation.
 *
 * This file includes the definitions for a thread-safe, buffered readline algorithm. It was originally
 * developed by Richard Stevens, but I did some small changes.
 */

#include "config.h"
#include "defines.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include "readline.h"

static ssize_t my_read(cf_rline_t *tsd,int fd,char *ptr) {
  int calls = 0;

  if(tsd->rl_cnt <= 0) {
    again:
    if((tsd->rl_cnt = read(fd,tsd->rl_buf,CF_BUFSIZ)) < 0) {
      if(errno == EINTR || errno == EAGAIN) {
        calls += 1;
        if(calls <= 3) goto again;
      }

      return -1;
    } else if(tsd->rl_cnt == 0) {
      return 0;
    }

    tsd->rl_bufptr = tsd->rl_buf;
  }

  tsd->rl_cnt--;
  *ptr = *tsd->rl_bufptr++;

  return 1;
}

char *cf_readline(int fd,cf_rline_t *tsd) {
  int  n,rc,len = CF_BUFSIZ;
  char c = '\0',*ptr,*line = malloc(CF_BUFSIZ);

  if(!line) return NULL;

  tsd->rl_mem = CF_BUFSIZ;
  tsd->rl_len = 0;

  ptr = line;
  for(n=1;c != '\n';n++) {
    if((rc = my_read(tsd,fd,&c)) == 1) {

      if(n >= len) {
        len += CF_BUFSIZ;

        line = realloc(line,len);
        if(!line) return NULL;

        ptr = &line[n-1];

        tsd->rl_mem = len;
      }

      *ptr++ = c;
      tsd->rl_len++;

      if(c == '\n') break;
    }
    else if(rc == 0) {
      if(n == 1) { /* no data read */
        free(line);
        return NULL;
      }
      else break;
    }
    else {
      free(line);
      return NULL;
    }
  }

  *ptr = '\0';
  return line;
}

ssize_t cf_writen(int fd,const void *vptr,size_t n) {
  size_t  nleft;
  ssize_t nwritten;
  const char *ptr;

  ptr   = vptr;
  nleft = n;
  while(nleft > 0) {
    if((nwritten = write(fd,ptr,nleft)) <= 0) {
      if(errno == EINTR || errno == EAGAIN) {
        nwritten = 0;
      }
      else {
        return -1;
      }
    }

    nleft -= nwritten;
    ptr   += nwritten;
  }

  return n;
}

/* eof */
