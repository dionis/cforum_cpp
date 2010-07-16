/*!
 * \file arrayutils.h
 * \author Christian Kruse, <cjk@wwwtech.de>
 * \brief Array interface for the Classic Forum
 *
 * An abstracted array implementation interface for the Classic Forum
 */

#ifndef CF_ARRAY_UTILS_H
#define CF_ARRAY_UTILS_H

/*! Array destructor function, called for every single element */
typedef void (*cf_array_destroy_t)(void *);

/*! Array comparator, used for sorting and searching */
typedef int (*cf_array_comparer_t)(const void *, const void *);

/*!
 * Array „class.”
 * This struct contains all necessary information about the array
 */
typedef struct s_array {
  size_t elements, /*!< Number of elements in this array instance */
         reserved, /*!< Amount of memory reserved */
         element_size, /*!< Size of one element */
         growth; /*!< Defines how may elements should be pre-reserved */

  cf_array_destroy_t cf_array_destroy; /*!< Function pointer to element destructor */
  unsigned char *array; /*!< Array pointer */
} cf_array_t;

#define CF_ARRAY_INITIALIZER(size,destroy) { 0, 0, (size), 0, (destroy), NULL }
#define CF_ARRAY_INITIALIZER_GROWTH(size,growth,destroy) { 0, 0, (size), (growth), (destroy), NULL }

#define cf_array_init(ary,element_size,array_destroy) cf_array_init_growth((ary),(element_size),(CF_PRERESERVE),(array_destroy))

/*!
 * This function initializes an array structure (it could be the constructor)
 * with a specific growth size
 * \param \c ary The array structure pointer
 * \param \c element_size The size of one element in the array
 * \param \c growth The size an array should grow in memory when size is to small for an operation
 * \param \c cf_array_destroy The destroy function for an array element. This function will be called whenever an element in the array has to be deleted. May be NULL if a function like this is not needed and a simple free() call is enough.
 */
void cf_array_init_growth(cf_array_t *ary,size_t element_size,size_t growth,cf_array_destroy_t array_destroy);

/*!
 * This function initializes an array structure (it could be the constructor)
 * with a specific growth size and an initial capacity
 * \param \c ary The array structure pointer
 * \param \c element_size The size of one element in the array
 * \param \c growth The size an array should grow in memory when size is to small for an operation
 * \param \c capacity The initial capacity
 * \param \c cf_array_destroy The destroy function for an array element. This function will be called whenever an element in the array has to be deleted. May be NULL if a function like this is not needed and a simple free() call is enough.
 */
void cf_array_init_capacity(cf_array_t *ary,size_t element_size,size_t growth, size_t capacity,cf_array_destroy_t cf_array_destroy);

/*!
 * This function pushes an element to the end of the array. The element
 * is being copied via a memdup() function, which only is a malloc() with
 * a memcpy().
 * \param \c ary The array structure pointer
 * \param \c element The element to push to the end
 */
void cf_array_push(cf_array_t *ary,const void *element);

/*!
 * This function deletes the last element in the array.
 * \param \c ary The array structure pointer
 * \return A copy of the 'popped' element
 * \attention Because this function returns a copy of this element, the destroy function will not be called!
 */
void *cf_array_pop(cf_array_t *ary);

/*!
 * This function deletes the first element in the array
 * \param \c ary The array structure pointer
 * \return \c A copy of the 'shifted' element
 * \attention Because this function returns a copy of this element, the destroy function will not be called!
 */
void *cf_array_shift(cf_array_t *ary);

/*!
 * This function inserts an element at the beginning of the array.
 * \param \c ary The array structure pointer
 * \param \c element The pointer to the element
 */
void cf_array_unshift(cf_array_t *ary,const void *element);

/*!
 * This function sorts an array via the quick sort algorithm
 * \param \c ary The array structure pointer
 * \param \c compar The comparing function. See qsort(3) for informations of the return values and arguments of this coparing function.
 */
void cf_array_sort(cf_array_t *ary,cf_array_comparer_t compar);

/*!
 * This function does a binary search on the array. Has to be sorted first!
 * \param \c ary The array structure pointer
 * \param \c key The key to search for
 * \param \c compar The comparing function. See bsearch(3) for information of the return values and the arguments of this comparing function.
 * \return Returns NULL if element not found or element if found
 */
void *cf_array_bsearch(cf_array_t *ary,const void *key,cf_array_comparer_t compar);

/*!
 * This function returns an element at a specified position.
 * \param \c ary The array structure pointer
 * \param \c index The index of the wanted element
 * \return The element at the specified position or NULL on failure
 */
void *cf_array_element_at(cf_array_t *ary,size_t index);

/*!
 * This function destroys an array. It calls the destroy function specified to cf_array_init()
 * for each argument and then free()s the array itself.
 * \param \c ary The array structure pointer
 */
void cf_array_destroy(cf_array_t *ary);

#endif

/* eof */
