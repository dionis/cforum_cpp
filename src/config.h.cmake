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

#define CF_FLOAT_EQ(x,v) ((((v) - (FLT_EPSILON)) < (x)) && ((x) < ((v) + (FLT_EPSILON))))
#define CF_DOUBL_EQ(x,v) ((((v) - (DBL_EPSILON)) < (x)) && ((x) < ((v) + (DBL_EPSILON))))

#define CF_MOD_MAGIC_MAJOR 4
#define CF_MOD_MAGIC_MINOR 0
#define CF_MOD_MAGIC ((CF_MOD_MAGIC_MAJOR<<8)|(CF_MOD_MAGIC_MINOR))

#define CF_VERSION "4.0a1"

#cmakedefine CF_PREFIX "${CF_PREFIX}"

#cmakedefine HAVE_STRDUP
#cmakedefine HAVE_STRNDUP
#cmakedefine HAVE_GETLINE
#cmakedefine HAVE_GETDELIM

#endif

/* eof */
