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

#define CF_DBL_EPSILON    2.2204460492503131e-016 /* smallest value for 1.0+DBL_EPSILON != 1.0 */
#define CF_FLT_EPSILON    1.192092896e-07F /* smallest value for 1.0+FLT_EPSILON != 1.0 */

#define CF_FLOAT_EQ(x,v) ((((v) - CF_FLT_EPSILON) < (x)) && ((x) < ((v) + CF_FLT_EPSILON)))
#define CF_DOUBL_EQ(x,v) ((((v) - CF_DBL_EPSILON) < (x)) && ((x) < ((v) + CF_DBL_EPSILON)))


#endif

/* eof */
