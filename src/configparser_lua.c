

#include "configparser.h"

#define CF_CFORUM_CLASS "CForum_"

/*!
 * Pushes a new configuration context on the stack
 * \param l The LUA state
 * \param conf The configuration context to push
 * \return the new configuration context pointer
 */
static cf_cfg_t **cf_cfg_l_pushcfg(lua_State *l,cf_cfg_t *conf) {
  cf_cfg_t **cfg = (cf_cfg_t **)lua_newuserdata(l, sizeof(*cfg));
  *cfg = conf;
  luaL_getmetatable(l, CF_CFORUM_CLASS);
  lua_setmetatable(l, -2);

  return cfg;
}

/*!
 * Converts a userdata to a cf_cfg_t
 * \param l The LUA state
 * \param index The index of the configuration context
 * \return Returns the converted configuration context
 */
static cf_cfg_t *cf_cfg_l_to_cfg(lua_State *l, int index) {
  cf_cfg_t **cfg = (cf_cfg_t **)lua_touserdata(l, index);
  if(cfg == NULL) luaL_typerror(l, index, CF_CFORUM_CLASS);
  return *cfg;
}

/*!
 * Converts a LUA array to internal configuration structures
 * \param l The LUA state
 * \param ary The array to store the values in
 */
static void cf_cfg_l_lary_to_cfary(lua_State *l,cf_array_t *ary) {
  cf_cfg_value_t val;
  int ival;
  double dval;

  val.name = NULL;

  lua_pushnil(l);  /* first key */
  while (lua_next(l, -2) != 0) {
    /* uses 'key' (at index -2) and 'value' (at index -1); we ignore key (for now) */

    if(lua_isboolean(l,-1)) {
      val.type = CF_CFG_VALUE_INT;
      val.value.ival = lua_toboolean(l,-1);
    }
    else if(lua_isnumber(l,-1)) {
      ival = luaL_checkinteger(l,-1);
      dval = luaL_checknumber(l,-1);

      if(CF_DOUBL_EQ(dval,(double)ival)) {
        val.type = CF_CFG_VALUE_DBL;
        val.value.dval = dval;
      }
      else {
        val.type = CF_CFG_VALUE_INT;
        val.value.ival = ival;
      }
    }
    else if(lua_isstring(l,-1)) {
      val.type = CF_CFG_VALUE_STR;
      val.value.cval = cf_to_utf16(luaL_checkstring(l,-1),-1,NULL);
    }
    else if(lua_istable(l,-1)) {
      val.type = CF_CFG_VALUE_ARY;

      val.value.aval = cf_alloc(NULL,1,sizeof(*val.value.aval),CF_ALLOC_MALLOC);
      cf_array_init(val.value.aval,sizeof(val),(cf_hash_cleanup_t)cf_cfg_destroy_value);

      cf_cfg_l_lary_to_cfary(l,val.value.aval);
    }

    cf_array_push(ary,&val);

    lua_pop(l, 1);
  }
}

/*!
 * LUA binding for storing config values
 * \param l The LUA state
 * \return Returns the number of values pushed on the stack
 */
static int cf_cfg_l_setvalue(lua_State *l) {
  cf_cfg_t *cfg = cf_cfg_l_to_cfg(l, 1);
  const char *name;
  cf_cfg_value_t val;
  double dval;
  int ival;

  if(!cfg) return luaL_error(l,"Error in LUA: %s",lua_tostring(l, -1));
  name = luaL_checkstring(l,-2);

  val.name = cf_to_utf16(name,-1,NULL);

  if(lua_isboolean(l,-1)) {
    val.type = CF_CFG_VALUE_INT;
    val.value.ival = lua_toboolean(l,-1);
  }
  else if(lua_isnumber(l,-1)) {
    ival = luaL_checkinteger(l,-1);
    dval = luaL_checknumber(l,-1);

    if(CF_DOUBL_EQ(dval,(double)ival)) {
      val.type = CF_CFG_VALUE_DBL;
      val.value.dval = dval;
    }
    else {
      val.type = CF_CFG_VALUE_INT;
      val.value.ival = ival;
    }
  }
  else if(lua_isstring(l,-1)) {
    val.type = CF_CFG_VALUE_STR;
    val.value.cval = cf_to_utf16(luaL_checkstring(l,-1),-1,NULL);
  }
  else if(lua_istable(l,-1)) {
    val.type = CF_CFG_VALUE_ARY;

    val.value.aval = cf_alloc(NULL,1,sizeof(*val.value.aval),CF_ALLOC_MALLOC);
    cf_array_init(val.value.aval,sizeof(val),(cf_hash_cleanup_t)cf_cfg_destroy_value);

    cf_cfg_l_lary_to_cfary(l,val.value.aval);
  }

  cf_hash_set(cfg->values,(char *)val.name,u_strlen(val.name) * sizeof(*val.name),&val,sizeof(val));

  return 0;
}

/*!
 * LUA binding to create a new context
 * \param l The LUA state
 * \return Returns the number of values pushed on the stack
 */
static int cf_cfg_l_createcontext(lua_State *l) {
  cf_cfg_t *cfg = cf_cfg_l_to_cfg(l, 1),ncfg,*ncfgp;
  const char *name = luaL_checkstring(l,-1);

  cf_cfg_init_cfg(&ncfg);
  ncfg.name = cf_to_utf16(name,-1,NULL);

  cf_array_push(&cfg->contexts,&ncfg);
  ncfgp = cf_array_element_at(&cfg->contexts,cfg->contexts.elements - 1);

  cf_cfg_l_pushcfg(l,ncfgp);

  return 1;
}

/*!
 * LUA binding: Converts a cf_cfg_t to a string
 * \param l The LUA state
 */
static int cf_cfg_to_s(lua_State *l) {
  lua_pushfstring(l, "<CForum:%p>", lua_touserdata(l, 1));
  return 1;
}

/*!
 * The cf_cfg_t methods
 */
static const luaL_reg CForum__methods[] = {
  { "set_value", cf_cfg_l_setvalue },
  { "create_context", cf_cfg_l_createcontext },
  { NULL, NULL }
};

/*!
 * The metatable for the CForum_ class (read LUA manual)
 */
static const luaL_reg CForum__meta[] = {
  { "__tostring", cf_cfg_to_s },
  { NULL, NULL }
};

void cf_cfg_l_register(lua_State *l,cf_cfg_t *conf) {
  luaL_openlib(l, CF_CFORUM_CLASS, CForum__methods, 0);  /* create methods table, add it to the globals */
  luaL_newmetatable(l, CF_CFORUM_CLASS); /* create metatable for Image, add it to the Lua registry */
  luaL_openlib(l, 0, CForum__meta, 0);
  lua_pushliteral(l, "__index");
  lua_pushvalue(l, -3); /* dup methods table*/
  lua_rawset(l, -3); /* metatable.__index = methods */
  lua_pushliteral(l, "__metatable");
  lua_pushvalue(l, -3); /* dup methods table*/
  lua_rawset(l, -3); /* hide metatable: metatable.__metatable = methods */
  lua_pop(l, 1); /* drop metatable */

  cf_cfg_l_pushcfg(l,conf);

  lua_setglobal(l,"CForum");
}

/* eof */
