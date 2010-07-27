/**
 * \file datautils.c
 * \author Christian Kruse, <cjk@wwwtech.de>
 * \brief Data type utilities for the Classic Forum
 *
 * Data type utilities such as int64 types to string and
 * vice versa
 */

#ifndef CF_DATAUTILS_H
#define CF_DATAUTILS_H

#include <unicode/uchar.h>

/*!
 * convert a string to an uint64
 * \param str The string
 * \param len The length of the string
 * \param errcode A pointer to an \c int where an error code would get stored on error, otherwise it'll be 0
 * \return The converted \c u_int64_t
 */
u_int64_t cf_uchr_to_uint64(const UChar *str,const int32_t len,int *errcode);

/*!
 * convert a string to an int64
 * \param str The string
 * \param len The length of the string
 * \param errcode A pointer to an \c int where an error code would get stored on error, otherwise it'll be 0
 * \return The converted \c int64_t
 */
int64_t cf_uchr_to_int64(const UChar *str,const int32_t len,int *errcode);

/*!
 * convert a u_int64_t to a string
 * \param str The string where the int should be stored
 * \param num The int
 */
int cf_uint64_to_str(cf_string_t *str, u_int64_t num);

/*!
 * convert a int64_t to a string
 * \param str The string where the int should be stored
 * \param num The int
 */
int cf_int64_to_str(cf_string_t *str, int64_t num);


#endif

/* eof */
