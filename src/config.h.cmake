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

/* C99 defines DBL_EPSILON & FLT_EPSILON in float.h */
#include <float.h>

#define CF_BUFSIZ 1024 /*!< Default buffer size */
#define CF_BUFSIZ_MEMPOOL CF_BUFSIZ /*!< Default memory pool buffer size */

#define CF_PRERESERVE 5 /*!< default prereservation count (e.g. for arrays) */

#define CF_FLOAT_EQ(x,v) ((((v) - FLT_EPSILON) < (x)) && ((x) < ((v) + FLT_EPSILON)))
#define CF_DOUBL_EQ(x,v) ((((v) - DBL_EPSILON) < (x)) && ((x) < ((v) + DBL_EPSILON)))

#define SIZE_T_FMT "%zd"

#define SCHEDULING SCHED_OTHER

#define CF_ERR_ILPARAM 1
#define CF_ERR_UNICODE 2

#define CF_LOG_ERROR  1
#define CF_LOG_WARN   2
#define CF_LOG_INFO   4
#define CF_LOG_NOTICE 8

#define CF_LOG_DBG(lvl) (1<<((lvl)+4))
#define CF_LOG_LOCK CF_LOG_DBG(3)

#define CF_SHALL_RUN 1
#define CF_SHALL_NOT_RUN 0

#define CF_MOD_MAGIC_MAJOR 4
#define CF_MOD_MAGIC_MINOR 0
#define CF_MOD_MAGIC ((CF_MOD_MAGIC_MAJOR<<8)|(CF_MOD_MAGIC_MINOR))

#define CF_LISTENQ 5

#cmakedefine CF_PREFIX "${CF_PREFIX}"

#cmakedefine HAVE_STRDUP
#cmakedefine HAVE_STRNDUP
#cmakedefine HAVE_GETLINE
#cmakedefine HAVE_GETDELIM

#endif

/* eof */
