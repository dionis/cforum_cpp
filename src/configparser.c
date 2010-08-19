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
    "./cforum.lua",
    "/etc/cforum.lua",
    "/usr/local/etc/cforum.lua",
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
  if(cfg->contexts.elements) cf_array_destroy(&cfg->contexts);
  if(cfg->modules.elements) cf_array_destroy(&cfg->modules);
  if(cfg->handlers.elements) cf_array_destroy(&cfg->handlers);
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

void cf_cfg_destroy_mod(cf_cfg_mod_t *mod) {
  if(mod->file) free(mod->file);
  if(mod->handle) dlclose(mod->handle);
}

/*! not yet used, but already planned in case of future enhancements */
void cf_cfg_destroy_handler(cf_cfg_mod_handler_t *hndl) {
  (void)hndl;
}

void cf_cfg_init_cfg(cf_cfg_t *cfg) {
  cf_array_init(&cfg->contexts,sizeof(*cfg),(cf_array_destroy_t)cf_cfg_destroy_cfg);
  cf_array_init(&cfg->modules,sizeof(cf_cfg_mod_t),(cf_array_destroy_t)cf_cfg_destroy_mod);
  cf_array_init(&cfg->handlers,sizeof(cf_cfg_mod_handler_t),(cf_array_destroy_t)cf_cfg_destroy_handler);

  cfg->values = cf_hash_new((cf_hash_cleanup_t)cf_cfg_destroy_value);
  cfg->name = NULL;
}

int cf_cfg_init_module(cf_cfg_t *cfg,cf_cfg_mod_t *mod) {
  return 0;
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
  int free_file = 0;

  if(filename == NULL) {
    file = cf_cfg_find_file();
    free_file = 1;
  }
  else file = (char *)filename;

  if(file == NULL) {
    fprintf(stderr,"Config file could not be found!\n");
    return NULL;
  }

  cfg = cf_cfg_new_cfg();
  cfg->name = cf_to_utf16(file,-1,NULL);

  l = luaL_newstate();
  luaL_openlibs(l);

  cf_cfg_l_register(l,cfg);

  if(luaL_dofile(l,file) != 0) {
    top = lua_gettop(l);
    fprintf(stderr,"Error in LUA: %s\n",lua_tostring(l,top));
    lua_close(l);
    cf_cfg_destroy_cfg(cfg);
    free(cfg);
    if(free_file) free(file);
    return NULL;
  }

  if(free_file) free(file);

  lua_close(l);
  return cfg;
}

cf_cfg_value_t *cf_cfg_get_value_c(cf_cfg_t *cfg,cf_cfg_contexts_t contexts,size_t clen,const char *name) {
  UChar *n = cf_to_utf16(name,-1,NULL);
  cf_cfg_value_t *val = cf_cfg_get_value_w_pos(cfg,contexts,clen,0,n);
  free(n);

  return val;
}

cf_cfg_value_t *cf_cfg_get_value_w_pos(cf_cfg_t *cfg,cf_cfg_contexts_t contexts,size_t clen, size_t pos,const UChar *name) {
  cf_cfg_value_t *val;
  cf_cfg_t *cont;
  size_t i;

  if(!cfg->values) return NULL;

  if(clen > 0 && cfg->contexts.elements > 0 && pos < clen && contexts != NULL) {
    for(i=0;i<cfg->contexts.elements;++i) {
      cont = cf_array_element_at(&cfg->contexts,i);

      if(u_strcmp(cont->name,contexts[pos]) == 0) {
        if((val = cf_cfg_get_value_w_pos(cont,contexts,clen,pos+1,name)) == NULL) break;
        return val;
      }
    }
  }

  return cf_hash_get(cfg->values,(char *)name,u_strlen(name) * sizeof(*name));
}

UChar **cf_cfg_create_contexts(const char **cnts,size_t num) {
  size_t i;
  UChar **ret;

  if(cnts == NULL || num <= 0) return NULL;

  ret = cf_alloc(NULL,num,sizeof(*ret),CF_ALLOC_MALLOC);
  for(i=0;i<num;++i) ret[i] = cf_to_utf16(cnts[i],-1,NULL);
  return ret;
}

void cf_cfg_destroy_contexts(UChar **cnts,size_t num) {
  size_t i;

  if(cnts == NULL || num <= 0) return;

  for(i=0;i<num;++i) {
    if(cnts[i]) free(cnts[i]);
  }

  free(cnts);
}

/*
int main(void) {
  char *cval;
  UChar **contexts;
  U_STRING_DECL(context,"abc",3);
  U_STRING_DECL(mpath,"ModulePath",10);

  cf_cfg_t *cfg = cf_cfg_read_config(NULL);
  cf_cfg_value_t *val;

  U_STRING_INIT(mpath,"ModulePath",10);

  contexts = cf_alloc(NULL,2,sizeof(*contexts),CF_ALLOC_MALLOC);

  U_STRING_INIT(context,"abc",3);
  contexts[0] = cf_strdup(context,-1);

  U_STRING_INIT(context,"def",3);
  contexts[1] = cf_strdup(context,-1);

  if(cfg) {
    if((val = cf_cfg_get_value(cfg,(const UChar **)contexts,2,mpath)) != NULL) {
      cval = cf_to_utf8(val->value.cval,-1,NULL);
      printf("val is: %s (%p)\n",cval,val);
      if(cval) free(cval);
    }

    cf_cfg_destroy_cfg(cfg);
    free(cfg);
  }

  free(contexts[0]);
  free(contexts[1]);
  free(contexts);

  return 0;
}*/

/* eof */
