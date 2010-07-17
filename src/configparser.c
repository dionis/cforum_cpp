/*!
 * \file configparser.c
 * \brief A configuration parser
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * This file contains some basic definements,
 * such as format strings, etc, pp
 */

#include "configparser.h"

char *cf_cfg_find_file(void) {
  static const char *files[] = {
    "./cforum.conf",
    "/etc/cforum.conf",
    "/usr/local/etc/cforum.conf",
    NULL
  };

  const char *path = getenv("CF_CONF_FILE");
  char *p = NULL;
  int i;
  struct stat st;

  if(path == NULL || stat(path,&st) != 0 || !S_ISREG(st.st_mode)) {
    path = NULL;

    for(i=0;files[i];++i) {
      if(stat(files[i],&st) == 0) {
        path = files[i];
        break;
      }
    }
  }

  if(path) p = strdup(path);
  return p;
}

void cf_cfg_destroy_cfg(cf_cfg_t *cfg) {
  if(cfg->contexts) {
    cf_array_destroy(cfg->contexts);
    free(cfg->contexts);
  }
  if(cfg->values) {
    cf_hash_destroy(cfg->values);
    free(cfg->values);
  }
  if(cfg->name) free(cfg->name);
}

void cf_cfg_destroy_value(cf_cfg_value_t *val) {
  if(val->name) free(val->name);
  if(val->type == CF_CFG_VALUE_STR) free(val->value.cval);
  if(val->type == CF_CFG_VALUE_ARY) {
    cf_array_destroy(val->value.aval);
    free(val->value.aval);
  }
}

void cf_cfg_init_cfg(cf_cfg_t *cfg) {
  cfg->contexts = cf_alloc(NULL,1,sizeof(*cfg->contexts),CF_ALLOC_MALLOC);
  cf_array_init(cfg->contexts,sizeof(*cfg),(cf_array_destroy_t)cf_cfg_destroy_cfg);
  cfg->values = cf_hash_new((cf_hash_cleanup_t)cf_cfg_destroy_value);
  cfg->name = NULL;
}

cf_cfg_t *cf_cfg_new_cfg(void) {
  cf_cfg_t *cfg = cf_alloc(NULL,1,sizeof(*cfg),CF_ALLOC_MALLOC);
  cf_cfg_init_cfg(cfg);

  return cfg;
}

cf_cfg_t *cf_cfg_read_config(const char *filename) {
  char *file;
  cf_cfg_t *cfg;
  lua_State *l;
  int top;

  if(filename == NULL) file = cf_cfg_find_file();
  else file = strdup(filename);

  if(file == NULL) {
    fprintf(stderr,"Config file could not be found!\n");
    return NULL;
  }

  cfg = cf_cfg_new_cfg();
  cfg->name = file;

  l = luaL_newstate();
  luaL_openlibs(l);

  cf_cfg_l_register(l,cfg);

  if(luaL_dofile(l,file) != 0) {
    top = lua_gettop(l);
    fprintf(stderr,"Error in LUA: %s\n",lua_tostring(l,top));
    lua_close(l);
    cf_cfg_destroy_cfg(cfg);
    free(cfg);
    return NULL;
  }

  lua_close(l);
  return cfg;
}

cf_cfg_value_t *cf_cfg_get_value_w_pos(cf_cfg_t *cfg,const char *contexts[],size_t clen, size_t pos,const char *name) {
  cf_cfg_value_t *val;
  cf_cfg_t *cont;
  size_t i;

  if(!cfg->values) return NULL;

  if(clen > 0 && cfg->contexts != NULL && cfg->contexts->elements > 0 && pos < clen) {
    for(i=0;i<cfg->contexts->elements;++i) {
      cont = cf_array_element_at(cfg->contexts,i);

      if(strcmp(cont->name,contexts[pos]) == 0) {
        if((val = cf_cfg_get_value_w_pos(cont,contexts,clen,pos+1,name)) == NULL) break;
        return val;
      }
    }
  }

  return cf_hash_get(cfg->values,name,strlen(name));
}

#if 0
int main(void) {
  const char *contexts[] = {"abc","def"};
  cf_cfg_t *cfg = cf_cfg_read_config(NULL);
  cf_cfg_value_t *val;

  if(cfg) {
    if((val = cf_cfg_get_value(cfg,contexts,2,"ModulePath")) != NULL) {
      printf("val is: %s (%p)\n",val->value.cval,val);
    }

    cf_cfg_destroy_cfg(cfg);
    free(cfg);
  }
  return 0;
}
#endif

/* eof */
