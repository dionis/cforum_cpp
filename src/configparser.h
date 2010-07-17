/*!
 * \file configparser.c
 * \brief A configuration parser
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * This file contains the interface definitions for the
 * Classic Forum config parser
 */

#ifndef CF_CONFIGPARSER_H
#define CF_CONFIGPARSER_H

#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "utils/arrayutils.h"
#include "utils/utils.h"
#include "hashlib.h"




#define CF_CFG_VALUE_STR 0x1 /*!< Config value is a string value */
#define CF_CFG_VALUE_INT 0x2 /*!< Config value is an int value */
#define CF_CFG_VALUE_DBL 0x3 /*!< Config value is a double value */
#define CF_CFG_VALUE_ARY 0x4 /*!< Config value is an array */

/*!
 * This struct contains configuration values. Config values can be
 * a string (CF_CFG_VALUE_STR), an integer (CF_CFG_VALUE_INT), a
 * double (CF_CFG_VALUE_DBL) or an array (may be of arrays,
 * CF_CFG_VALUE_ARY)
 */
typedef struct cf_cfg_value_s {
  char type; /*!< The value type */
  char *name; /*!< The value name (if given) */
  union {
    char *cval; /*!< string data */
    int ival; /*!< int data */
    double dval; /*!< double data */
    cf_array_t *aval; /*!< array data */
  } value; /*!< the value itself */
} cf_cfg_value_t;

/*!
 * This type holds the entire configuration. A configuration consists of
 * contexts (which can contain contexts, too) and values (name-value pairs).
 * Each value is a cf_cfg_value_t, each context is a cf_cfg_t
 */
typedef struct cf_cfg_s {
  char *name; /*!< the name of the context. config filename in global (configuration) context */
  cf_array_t *contexts; /*!< The list of available contexts */
  cf_hash_t *values; /*!< the values in this context */
} cf_cfg_t;

/*!
 * Find our config file.
 * \returns \c NULL if no file could be found or the filename with path if file could be found.
 * \attention Remember to free the return value if not NULL!
 */
char *cf_cfg_find_file(void);

/*!
 * Destroy a cf_cfg_t structure
 * \param cfg The config structure
 */
void cf_cfg_destroy_cfg(cf_cfg_t *cfg);

/*!
 * Initialize a configuration structure
 * \param cfg The (allocated) cf_cfg_t
 */
void cf_cfg_init_cfg(cf_cfg_t *cfg);

/*!
 * Allocate and initialize a new cf_cfg_t structure
 */
cf_cfg_t *cf_cfg_new_cfg(void);

/*!
 * Read a configuration file
 * \param filename The file to read, may be NULL in wich case cf_cfg_find_file is called
 * \return NULL on failure, the allocated cf_cfg_t structure on success
 */
cf_cfg_t *cf_cfg_read_config(const char *filename);

/*!
 * register the cforum config bindings in LUA
 * \param l The LUA state in which we should register
 * \param conf The config we sould set as the global CForum
 */
void cf_cfg_l_register(lua_State *l,cf_cfg_t *conf);

/*!
 * Destroys a configuration value
 * \param val The configuration value to destroy
 */
void cf_cfg_destroy_value(cf_cfg_value_t *val);

#endif

/* eof */
