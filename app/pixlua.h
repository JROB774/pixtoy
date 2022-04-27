#ifndef PIXLUA_H__ /*/////////////////////////////////////////////////////////*/
#define PIXLUA_H__

#define LUA_CORE
#define LUA_LIB

#include <lprefix.h>
#include <luaconf.h>

// We redefine LUA_API to have the same definition type as our functions.
#undef  LUA_API
#define LUA_API PIXDEF

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#endif /* PIXLUA_H__ /////////////////////////////////////////////////////////*/
