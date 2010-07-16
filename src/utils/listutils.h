/**
 * \file listutils.h
 * \author Christian Kruse, <cjk@wwwtech.de>
 * \brief double linked list abstraction interface for the Classic Forum
 *
 * double linked list abstraction interface for the Classic Forum
 */

#ifndef CF_LIST_UTILS_H
#define CF_LIST_UTILS_H

/*! Type to destroy list elements. Gets called for every element. */
typedef void (*cf_list_destroy_t)(void *data);

/*! Type to compare list elements, used for searching */
typedef int (*cf_list_comparer_t)(const void *data1,const void *data2);

/*!
 * This structure defines an element of the doubly linked list
 */
typedef struct cf_list_element_s {
  void *data; /*!< saved data */
  size_t size; /*!< size of the data field */
  int type; /*!< static or on heap: 1 if static */

  struct cf_list_element_s *prev; /*!< pointer to the previous element */
  struct cf_list_element_s *next; /*!< pointer to the next element */
} cf_list_element_t;

/*!
 * This structure defines the header for a doubly linked list
 */
typedef struct cf_list_head_s {
  cf_list_element_t *elements; /*!< list elements pointer */
  cf_list_element_t *last; /*!< pointer to the last element */
} cf_list_head_t;


/*!
 * This function initializes a list head
 * \param head The list header variable
 */
void cf_list_init(cf_list_head_t *head);

/*!
 * This function appends an element to a list
 * \param head The list header variable for this list
 * \param data The data to append
 * \param size The size of the data
 */
void cf_list_append(cf_list_head_t *head,void *data,size_t size);

/*!
 * This function appends an element to a list _and_does_not_copy_it_ but
 * safes the data argument as a reference
 * \param head The list header variable for this list
 * \param data The data to append
 * \param size The size of the data
 */

void cf_list_append_static(cf_list_head_t *head,void *data,size_t size);

/*!
 * This function prepends an element to a list
 * \param head The list header variable for this list
 * \param data The data to prepend
 * \param size The size of the data
 */
void cf_list_prepend(cf_list_head_t *head,void *data,size_t size);

/*!
 * This function prepends an element to a list _and_does_not_copy_it
_
 * but safes the data argument as a reference
 * \param head The list header variable for this list
 * \param data The data to prepend
 * \param size The size of the data
 */

void cf_list_prepend_static(cf_list_head_t *head,void *data,size_t size);

/*!
 * This function inserts an element in a list after the given element
 * \param head The list header variable for this list
 * \param prev The list element variable to insert after
 * \param data The data to insert
 * \param size The size of the data
 */
void cf_list_insert(cf_list_head_t *head,cf_list_element_t *prev,void *data,size_t size);

/*!
 * This function searches an element in a list
 * \param head The list header variable for this list
 * \param data The data to search
 * \param compare The comparing function
 * \return Returns the data of the element if found or NULL if not found
 */
void *cf_list_search(cf_list_head_t *head,void *data,cf_list_comparer_t compare);

/*!
 * This function deletes an element from a list
 * \param head The list header variable for this list
 * \param elem The list element to delete
 */
void cf_list_delete(cf_list_head_t *head,cf_list_element_t *elem);

/*!
 * This function destroys a list
 * \param head The list header variable for this list
 * \param destroy A destroying function for the list elements (NULL if not needed)
 */
void cf_list_destroy(cf_list_head_t *head,cf_list_destroy_t destroy);

#endif

/* eof */
