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

/*!
 * convert a string to an uint64
 * \param ptr The string
 * \return The converted \c u_int64_t
 */
u_int64_t cf_str_to_uint64(register const char *ptr);

/*!
 * convert a u_int64_t to a string
 * \param str The string where the int should be stored
 * \param num The int
 */
void cf_uint64_to_str(cf_string_t *str, u_int64_t num);

/*!
 * convert a u_int32_t to a string
 * \param str The string where the int should be stored
 * \param num The int
 */
void cf_uint32_to_str(cf_string_t *str, u_int32_t num);

/*!
 * convert a u_int16_t to a string
 * \param str The string where the int should be stored
 * \param num The int
 */
void cf_uint16_to_str(cf_string_t *str, u_int16_t num);

#endif

/* eof */
