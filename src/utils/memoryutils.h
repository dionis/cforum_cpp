/*!
 * \file memoryutils.h
 * \brief Interface for memory utilities for the Classic Forum, making the life easier
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * Interface for the memory utilities for the Classic Forum, such as memdup(), a memory pool
 * abstraction, etc, pp
 */

#ifndef CF_MEMORY_UTILS_H
#define CF_MEMORY_UTILS_H

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>

/*!
 * \brief This function duplicates a memory area
 * \param inptr The pointer to the original memory area
 * \param size The size of the memory area
 * \return A pointer to the duplicated memory area
 */
void *memdup(void *inptr,size_t size);


#define CF_ALLOC_MALLOC  0x0 /*!< use malloc() for memory allocation */
#define CF_ALLOC_CALLOC  0x1 /*!< use calloc() for memory allocation */
#define CF_ALLOC_REALLOC 0x2 /*!< use realloc() for memory allocation */

/*!
 * \brief Safely allocates new memory
 * \param ptr The old pointer for realloc()
 * \param nmemb The number of objects to allocate
 * \param size The size of one object
 * \param type The type of the allocation (CF_ALLOC_MALLOC, CF_ALLOC_CALLOC or CF_ALLOC_REALLOC)
 * \return The new pointer
 */
void *cf_alloc(void *ptr,size_t nmemb,size_t size,int type);


/*!
 * This is the heart of the memory abstraction. It contains the memory area itself, the length
 * of the memory used and the length of the area itself
 */
typedef struct s_mem_pool {
  unsigned long len; /*!< length of the memory used */
  unsigned long reserved; /*!< length of the memory area itself */
  unsigned growth; /*!< The growth factor */
  unsigned char *content; /*!< The memory area itself */
} cf_mem_pool_t;

/*! Initializer macro */
#define CF_MEMPOOL_INITIALIZER { 0, 0, 0, NULL }

/*!
 * \brief This function initializes a memory pool
 * \param pool The memory pool object pointer
 */
#define cf_mem_init(pool) (cf_mem_init_growth((pool),CF_BUFSIZ_MEMPOOL))

/*!
 * \brief This function initializes a memory pool with a specified growth parameter
 * \param pool The memory pool object pointer
 * \param growth The growth factor
 */
void cf_mem_init_growth(cf_mem_pool_t *pool,size_t growth);

/*!
 * \brief This function cleans up a memory pool.
 * \param pool The memory pool object pointer
 */
void cf_mem_cleanup(cf_mem_pool_t *pool);

/*!
 * \brief This function sets the content of a memory pool
 * \param pool The memory pool object pointer
 * \param src The source pointer
 * \param length The length of the region to copy
 * \return Number of copied bytes
 */
size_t cf_mem_set(cf_mem_pool_t *pool,const unsigned char *src,size_t length);

/*!
 * \brief This function appends a memory region to the region in the pool.
 * \param pool The memory pool
 * \param src The source pointer
 * \param len The length of the region
 * \return A pointer to the beginning of the copied data in the pool
 */
unsigned char *cf_mem_append(cf_mem_pool_t *pool,const unsigned char *src,size_t len);


#endif

/* eof */
