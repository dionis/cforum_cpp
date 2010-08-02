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

#include <unicode/uchar.h>
#include <unicode/ustring.h>

#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "utils/stringutils.h"
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
  UChar *name; /*!< The value name (if given) */
  union {
    UChar *cval; /*!< string data */
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
  UChar *name; /*!< the name of the context. config filename in global (configuration) context */
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


/*!
 * Access function for config values. It works with „contexts.” That means you define
 * which contexts may be active (e.g. FORUM1, fo_server) and the function will first
 * look for the value in the context fo_server in the context FORUM1. If it cannot
 * be found it looks in the context FORUM1 and after that in the global context. This
 * leads to a hierarchical structure <global>\FORUM1\fo_server
 *
 * \param cfg The configuration
 * \param contexts The contexts of the program
 * \param clen The length of the contexts array
 * \param pos The position in the contexts
 * \param name The name of the value to find
 * \return returns NULL if value could not be found, returns the value if the value could be found
 */
cf_cfg_value_t *cf_cfg_get_value_w_pos(cf_cfg_t *cfg,const UChar **contexts,size_t clen, size_t pos,const UChar *name);

#define cf_cfg_get_value(cfg,contexts,clen,name) (cf_cfg_get_value_w_pos((cfg),(contexts),(clen),0,(name)))

/*!
 * Convenience function to access config values with const char * literals, calls cf_cfg_get_value_w_pos()
 * \param cfg The configuration
 * \param contexts The contexts of the program
 * \param clen The length of the contexts array
 * \param name The name of the value to find
 * \return returns NULL if value could not be found or the name could not be converted to UTF-16, returns the value if the value could be found
 */
cf_cfg_value_t *cf_cfg_get_value_c(cf_cfg_t *cfg,const UChar **contexts,size_t clen,const char *name);

/*!
 * Function to create an UChar ** array of given char ** contexts
 * \param cnts The contexts
 * \param num The number of contexts to create
 * \return returns NULL if cnts is NULL or num <= 0, returns the UChar ** array otherwise
 */
UChar **cf_cfg_create_contexts(const char **cnts,size_t num);

/*!
 * Function to destroy an UChar ** array of contexts
 * \param cnts The contexts
 * \param num The number of contexts to destroy
 */
void cf_cfg_destroy_contexts(UChar **cnts,size_t num);

#endif

/* eof */
