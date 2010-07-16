/*!
 * \file arrayutils.c
 * \author Christian Kruse, <cjk@wwwtech.de>
 * \brief Array implementation for the Classic Forum
 *
 * An abstracted array implementation for the Classic Forum
 */

#include "arrayutils.h"

void cf_array_init_growth(cf_array_t *ary,size_t element_size,size_t growth,cf_array_destroy_t cf_array_destroy) {
  ary->reserved      = 0;
  ary->elements      = 0;
  ary->element_size  = element_size;
  ary->growth        = growth;
  ary->array         = NULL;
  ary->cf_array_destroy = cf_array_destroy;
}

void cf_array_init_capacity(cf_array_t *ary,size_t element_size,size_t growth, size_t capacity,cf_array_destroy_t cf_array_destroy) {
  ary->reserved      = capacity;
  ary->elements      = 0;
  ary->element_size  = element_size;
  ary->growth        = growth;
  ary->array         = cf_alloc(NULL,capacity,element_size,CF_ALLOC_MALLOC);
  ary->cf_array_destroy = cf_array_destroy;
}

void cf_array_push(cf_array_t *ary,const void *element) {
  if(ary->elements + 1 >= ary->reserved) {
    ary->array     = cf_alloc(ary->array,ary->element_size,ary->reserved+ary->growth,CF_ALLOC_REALLOC);
    ary->reserved += ary->growth;
  }

  memcpy(ary->array + (ary->elements * ary->element_size),element,ary->element_size);
  ary->elements += 1;
}

void *cf_array_pop(cf_array_t *ary) {
  ary->elements -= 1;
  return memdup((void *)(ary->array + ((ary->elements) * ary->element_size)),ary->element_size);
}

void *cf_array_shift(cf_array_t *ary) {
  void *elem = memdup(ary->array,ary->element_size);

  memmove(ary->array,ary->array+ary->element_size,(ary->elements - 1) * ary->element_size);
  ary->elements -= 1;
  return elem;
}

void cf_array_unshift(cf_array_t *ary,const void *element) {
  if(ary->elements + 1 >= ary->reserved) {
    ary->array     = cf_alloc(ary->array,ary->element_size,ary->reserved+ary->growth,CF_ALLOC_REALLOC);
    ary->reserved += ary->growth;
  }

  memmove(ary->array+ary->element_size,ary->array,ary->elements  * ary->element_size);
  memcpy(ary->array,element,ary->element_size);
  ary->elements += 1;
}

void cf_array_sort(cf_array_t *ary,cf_array_comparer_t compar) {
  qsort(ary->array,ary->elements,ary->element_size,compar);
}

void *cf_array_bsearch(cf_array_t *ary,const void *key,cf_array_comparer_t compar) {
  return bsearch(key,ary->array,ary->elements,ary->element_size,compar);
}

void *cf_array_element_at(cf_array_t *ary,size_t index) {
  if(index >= ary->elements) {
    errno = EINVAL;
    return NULL;
  }

  return ary->array + (index * ary->element_size);
}

void cf_array_destroy(cf_array_t *ary) {
  size_t i;

  if(ary->cf_array_destroy) {
    for(i=0;i<ary->elements;i++) {
      ary->cf_array_destroy(ary->array + (i * ary->element_size));
    }
  }

  free(ary->array);
  memset(ary,0,sizeof(*ary));
}

/* eof */
