/*!
 * \file validate.h
 * \author Christian Kruse, <cjk@wwwtech.de>
 * \brief Validation functions for several URI types
 *
 * This file contains validation functions for several
 * URI types (e.g. link and mail validation)
 *
 */

#ifndef CF_VALIDATE_H
#define CF_VALIDATE_H

#include "config.h"
#include "defines.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <idna.h>

/*!
 * scheme list data structure
 */
typedef struct cf_scheme_list_s {
  char scheme[20]; /*!< The name of the scheme */
  int (*validator)(const char *); /*!< Pointer to the validation function */
} cf_scheme_list_t;

/*!
 * General link checking function. Tries to get the scheme and calls the
 * validation function for that scheme
 * \param link The link to check
 * \return 0 if link is valid, -1 if it isn't
 */
int cf_is_valid_link(const char *link);

/*!
 * Validation function for HTTP links
 * \param link The link to check
 * \param strict Strict mode or no strict mode. If strict, no anchors are allowed
 * \return 0 if link is valid, -1 if it isn't
 */
int cf_is_valid_http_link(const char *link,int strict);

/*!
 * Validation function for a mail address
 * \param addr The mail address
 * \return 0 if the address is valid, -1 if it isn't
 */
int cf_is_valid_mailaddress(const char *addr);

/*!
 * Checks if a link is a valid mailto link
 * \param addr The link string to check
 * \return 0 if the address is valid, -1 if it isn't
 */
int cf_is_valid_mailto_link(const char *addr);

/*!
 * Checks if a hostname is valid
 * \param hostname The name of the host
 * \return 0 if it is valid, -1 if it isn't
 */
int cf_is_valid_hostname(const char *hostname);

/*!
 * Checks if a link is a valid telnet link
 * \param link The link string to check
 * \return 0 if the address is valid, -1 if it isn't
 */
int cf_is_valid_telnet_link(const char *link);

/*!
 * Checks if a link is a valid nntp link
 * \param link The link string to check
 * \return 0 if the address is valid, -1 if it isn't
 */
int cf_is_valid_nntp_link(const char *link);

/*!
 * Checks if a link is a valid news link
 * \param link The link string to check
 * \return 0 if the address is valid, -1 if it isn't
 */
int cf_is_valid_news_link(const char *link);

/*!
 * Checks if a link is a valid ftp link
 * \param link The link string to check
 * \return 0 if the address is valid, -1 if it isn't
 */
int cf_is_valid_ftp_link(const char *link);

/*
 * by Christian Seiler:
 */

/*!
 * Checks if a link is a valid prospero link
 * \param link The link string to check
 * \return 0 if the address is valid, -1 if it isn't
 */
int cf_is_valid_prospero_link(const char *link);

/*!
 * Checks if a link is a valid wais link
 * \param link The link string to check
 * \return 0 if the address is valid, -1 if it isn't
 */
int cf_is_valid_wais_link(const char *link);

/*!
 * Checks if a link is a valid gopher link
 * \param link The link string to check
 * \return 0 if the address is valid, -1 if it isn't
 */
int cf_is_valid_gopher_link(const char *link);

/*!
 * Checks if a link is a valid file:// link
 * \param link The link string to check
 * \return 0 if the address is valid, -1 if it isn't
 */
int cf_is_valid_file_link(const char *link);

#endif

/* eof */
