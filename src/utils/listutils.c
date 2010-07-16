/**
 * \file listutils.c
 * \author Christian Kruse, <cjk@wwwtech.de>
 * \brief double linked list abstraction for the Classic Forum
 *
 * double linked list abstraction for the Classic Forum
 */

#include "listutils.h"


void cf_list_init(cf_list_head_t *head) {
  memset(head,0,sizeof(*head));
}

void cf_list_append(cf_list_head_t *head,void *data,size_t size) {
  cf_list_element_t *elem = cf_alloc(NULL,1,sizeof(*elem),CF_ALLOC_CALLOC);

  elem->data = memdup(data,size);
  elem->size = size;

  if(head->last == NULL) {
    head->last = head->elements = elem;
  }
  else {
    head->last->next = elem;
    elem->prev       = head->last;
    head->last       = elem;
  }
}

void cf_list_append_static(cf_list_head_t *head,void *data,size_t size) {
  cf_list_element_t *elem = cf_alloc(NULL,1,sizeof(*elem),CF_ALLOC_CALLOC);

  elem->data = data;
  elem->size = size;
  elem->type = 1;

  if(head->last == NULL) {
    head->last = head->elements = elem;
  }
  else {
    head->last->next = elem;
    elem->prev       = head->last;
    head->last       = elem;
  }
}

void cf_list_prepend(cf_list_head_t *head,void *data,size_t size) {
  cf_list_element_t *elem = cf_alloc(NULL,1,sizeof(*elem),CF_ALLOC_CALLOC);

  elem->data = memdup(data,size);
  elem->size = size;

  if(head->elements) {
    elem->next = head->elements;
    head->elements->prev = elem;
    head->elements = elem;
  }
  else {
    head->last = head->elements = elem;
  }
}

void cf_list_prepend_static(cf_list_head_t *head,void *data,size_t size) {
  cf_list_element_t *elem = cf_alloc(NULL,1,sizeof(*elem),CF_ALLOC_CALLOC);

  elem->data = data;
  elem->size = size;
  elem->type = 1;

  if(head->elements) {
    elem->next = head->elements;
    head->elements->prev = elem;
    head->elements = elem;
  }
  else {
    head->last = head->elements = elem;
  }
}

void cf_list_insert(cf_list_head_t *head,cf_list_element_t *prev,void *data,size_t size) {
  cf_list_element_t *elem = cf_alloc(NULL,1,sizeof(*elem),CF_ALLOC_CALLOC);

  (void)head; /* not used, just for convenience */

  elem->data = memdup(data,size);
  elem->size = size;

  elem->next = prev->next;
  elem->prev = prev;
  if(prev->next) prev->next->prev = elem;
  prev->next = elem;
}

void *cf_list_search(cf_list_head_t *head,void *data,cf_list_comparer_t compare) {
  cf_list_element_t *elem;

  for(elem=head->elements;elem;elem=elem->next) {
    if(compare(elem->data,data) == 0) return elem->data;
  }

  return NULL;
}

void cf_list_delete(cf_list_head_t *head,cf_list_element_t *elem) {
  if(elem->prev) elem->prev->next = elem->next;
  if(elem->next) elem->next->prev = elem->prev;

  if(head->elements == elem) head->elements = elem->next;
  if(head->last == elem) head->last = elem->next;
}

void cf_list_destroy(cf_list_head_t *head,cf_list_destroy_t destroy) {
  cf_list_element_t *elem,*elem1;

  for(elem=head->elements;elem;elem=elem1) {
    elem1 = elem->next;

    if(destroy) destroy(elem->data);
    if(elem->type == 0) free(elem->data);
    free(elem);
  }
}

/* eof */
