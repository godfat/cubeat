#ifndef __SHOOTING_CUBES_LUA_UTILITY__
#define __SHOOTING_CUBES_LUA_UTILITY__

#include <cstdio>
#include <cstdlib>
#include <lua.hpp>

void error(lua_State* L, char const* fmt, ...);
void stack_dump(lua_State* L);

#endif// __SHOOTING_CUBES_LUA_UTILITY__
