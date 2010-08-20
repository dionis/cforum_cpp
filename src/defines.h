/**
 * \file config.h
 * \brief Some basic defines
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * This file contains some basic definements,
 * such as format strings, etc, pp
 */

#ifndef CF_DEFINES_H
#define CF_DEFINES_H

#define SIZE_T_FMT "%zd"


#define CF_ERR_ILPARAM 1
#define CF_ERR_UNICODE 2

#define CF_DEBUG

#define CF_LOG_ERROR  1
#define CF_LOG_WARN   2
#define CF_LOG_INFO   4
#define CF_LOG_NOTICE 8

#define CF_LOG_DBG(lvl) (1<<((lvl)+4))
#define CF_LOG_LOCK CF_LOG_DBG(3)

#define CF_SHALL_RUN 1
#define CF_SHALL_NOT_RUN 0


#endif

/* eof */
