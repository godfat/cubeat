
#include "utils/lua_utility.hpp"

using namespace psc;
using namespace script;

bool Lua::run_script(lua_State* L, char const* filename) {
    if( luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0) ) {
        //bad call will return non-zero
        error(L, "cannot load file: \n  %s", lua_tostring(L, -1) );
        return true;
    }
    return false;
}

bool Lua::eval(lua_State* L, char const* script) {
    if( luaL_loadstring(L, script) || lua_pcall(L, 0, LUA_MULTRET, 0) ) {
        error(L, "eval failed: \n  %s", lua_tostring(L, -1) );
        return true;
    }
    return false;
}

void Lua::error(lua_State* L, char const* fmt, ...) {
    va_list argp;
    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);
    printf("\n");
    lua_close(L);
    //exit(EXIT_FAILURE);
}

void Lua::stack_dump(lua_State* L)
{
    int top = lua_gettop(L);
    for(int i = 1; i <= top; ++i) {
        int t = lua_type(L, i);
        printf("%-5d%s\t", i, lua_typename(L, t));
        switch(t) {
            case LUA_TSTRING:
                printf("'%s'", lua_tostring(L, i));
                break;
            case LUA_TNUMBER:
                printf("%g", lua_tonumber(L, i));
                break;
            case LUA_TBOOLEAN:
                printf(lua_toboolean(L, i) ? "true" : "false");
                break;
            default:
                printf("%s", lua_typename(L, t));
                break;
        }
        printf("\n");
    }
    printf("\n");
}

