/**
 * \file config.h
 * \brief Some basic configuration
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * This file contains some basic configuration,
 * such as default buffer sizes, etc, pp
 */

#ifndef CF_CONFIG_H
#define CF_CONFIG_H

#define CF_BUFSIZ 1024 /*!< Default buffer size */
#define CF_BUFSIZ_MEMPOOL CF_BUFSIZ /*!< Default memory pool buffer size */

#define CF_PRERESERVE 5 /*!< default prereservation count (e.g. for arrays) */

#endif

/* eof */
