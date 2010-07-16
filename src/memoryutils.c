/*!
 * \file memoryutils.c
 * \brief Memory utilities for the Classic Forum, making the life easier
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * Memory utilities for the Classic Forum, such as memdup(), a memory pool
 * abstraction, etc, pp
 */

#include "defines.h"
#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>

#include "memoryutils.h"


void *memdup(void *inptr,size_t size) {
  void *outptr = cf_alloc(NULL,1,size,CF_ALLOC_MALLOC);
  memcpy(outptr,inptr,size);

  return outptr;
}


void *cf_alloc(void *ptr,size_t nmemb,size_t size,int type) {
  void *l_ptr = NULL;

  switch(type) {
    case CF_ALLOC_CALLOC:
      l_ptr = calloc(nmemb,size);
      break;
    case CF_ALLOC_REALLOC:
      l_ptr = realloc(ptr,size * nmemb);
      break;

    case CF_ALLOC_MALLOC:
    default:
      l_ptr = malloc(nmemb * size);
      break;
  }

  if(!l_ptr) {
    fprintf(stderr,"memory utils: error allocating "SIZE_T_FMT" bytes of memory: %s\n",nmemb * size,strerror(errno));
    exit(EXIT_FAILURE);
  }

  return l_ptr;
}


void cf_mem_cleanup(cf_mem_pool_t *pool) {
  pool->len      = 0;
  pool->reserved = 0;

  if(pool->content) free(pool->content);

  pool->content  = NULL;
}

unsigned char *cf_mem_append(cf_mem_pool_t *pool,const unsigned char *src,size_t length) {
  size_t len = pool->growth;

  if(pool->len + length >= pool->reserved) {
    if(length >= len) len += length;

    pool->reserved += len;
    pool->content   = cf_alloc(pool->content,(size_t)pool->reserved,1,CF_ALLOC_REALLOC);
  }

  memcpy(pool->content + pool->len,src,length);
  pool->len += length;

  return pool->content + pool->len - length;
}

size_t cf_mem_set(cf_mem_pool_t *pool,const unsigned char *src,size_t length) {
  size_t len = pool->growth;

  if(pool->len + length >= pool->reserved) {
    if(length >= len) len += length;

    pool->content   = cf_alloc(pool->content,pool->reserved + len,1,CF_ALLOC_REALLOC);
    pool->reserved += len;
  }

  memcpy(pool->content,src,length);
  pool->len = length;

  return length;
}


/* eof */
