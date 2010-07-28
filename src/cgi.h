/*!
 * \file cfcgi.h
 * \author Christian Kruse, <cjk@wwwtech.de>
 * \brief Classic Forum CGI-Library
 *
 * This is a small but hopefully fast CGI library. It was
 * developed within the Classic Forum project, <http://github.com/ckruse/cforum>
 *
 * It can handle POST and GET requests, but no multipart/form-data requests.
 * It can handle multiple form values (form fields with the same names, e.g.
 * checkboxes or something like that). Within these basic conditions it should
 * work very fast and reliable.
 *
 * This library consists of functions around a hash which contains the CGI parameters. So
 * the Classic Forum hash library is also needed.
 */

#ifndef CF_CGI_H
#define CF_CGI_H

#include "config.h"
#include "defines.h"

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "hashlib.h"
#include "utils/memoryutils.h"
#include "utils/arrayutils.h"

/*!
 * this type is used for multiple form values (such as checkboxes or something like that)
 */
typedef struct s_cf_cgi_param {
  char *name; /*!< the parameter name */
  char *value; /*!< the parameter value */
  struct s_cf_cgi_param *next; /*!< the values are stored in a double linked list. This is the pointer to the element after the actual element. It is NULL if there is no element after. */
  struct s_cf_cgi_param *last; /*!< the values are stored in a double linked list. This is the pointer to the element before the actual element. It is NULL if there is no element before. */
} cf_cgi_param_t;

/*!
 * this function is the constructor for the CGI hash. It reads the CGI data
 * from the environment, parses it and decodes it.
 * \return It returns NULL on error or the CGI hash on success.
 */
cf_hash_t *cf_cgi_new(void);

/*!
 * This function creates a CGI hash from a PATH_INFO string. It expects the
 * PATH_INFO string to be name-value pairs
 * \param hash If not NULL, this hash will be used to store the parameters
 * \return It returns NULL on error or the CGI hash on success.
 */
cf_hash_t *cf_cgi_parse_path_info_nv(cf_hash_t *hash);

/*!
 * This function parses a query string
 * \param ary A pointer to a array structure
 */
void cf_cgi_parse_path_info(cf_array_t *ary);

/*!
 * This function encodes a string to an URL encoded string. This means,
 * all characters < 48, > 122 except '_', '.' and '-' will be encoded to '%<2 digit hexvalue of character>'.
 * Space will be converted to '+'.
 * \param str the string as a C string
 * \param len the length of the string
 * \return It returns NULL on failure (e.g. out of memory or something like that) and the decoded string on success.
 * \attention You have to free() the char array!
 */
char *cf_cgi_url_encode(const char *str,size_t len);

/*!
 * This function decodes an URL encoded string. This means, %<2-digit-hexvalue> will be decoded to the
 * corresponding character
 * \param str the URL encoded string as a C string
 * \param len the length of the string
 * \return It returns NULL on failure (e.g. out of memory  or something like that) and the decoded string on success.
 */
char *cf_cgi_url_decode(const char *str,size_t len);

/*!
 * This function gets a single CGI value and returns it as a C string.
 * \attention You may not free this returned value! This will be done when you call cf_cgi_destroy().
 */
char *cf_cgi_get(cf_hash_t *cgi,const char *name);

/*!
 * This function sets a CGI param value
 * \param hash The CGI object hash
 * \param name The parameter name
 * \param value The value of the parameter
 */
void cf_cgi_set(cf_hash_t *hash,const char *name,const char *value);

/*!
 * This function returns a double linked list of parameters with the name the parameter 'param' defines.
 * \param hash  the CGI hash
 * \param param the parameter name
 * \attention You may not free() anything in this list! This is done by the cf_cgi_destroy_entry() function!
 * \return The linked list if the entry could be found, NULL if the entry was not found.
 */
cf_cgi_param_t *cf_cgi_get_multiple(cf_hash_t *hash,char *param);

/*!
 * This function destroys a CGI hash.
 * \param hash The CGI hash
 */
void cf_cgi_destroy(cf_hash_t *hash);

/*!
 * This function parses a PATH_INFO string
 * \param infos A reference to a char ** array
 * \return Returns the length of the char ** array
 */
u_int32_t cf_cgi_path_info_parsed(char ***infos);

/*!
 * This function parses the cookies
 * \param hash A hash to save cookies in
 */
void cf_cgi_parse_cookies(cf_hash_t *hash);

/*!
 * this function destroys a parameter list.
 * \param data the given parameter list
 */
void cf_cgi_destroy_entry(void *data);

#endif

/* eof */
