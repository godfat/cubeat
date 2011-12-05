#ifndef __SHOOTING_CUBES_LUA_UTILITY__
#define __SHOOTING_CUBES_LUA_UTILITY__

#include <cstdio>
#include <cstdlib>
#include <lua.hpp>

namespace psc {
namespace script {

struct Lua
{
    static void error(lua_State* L, char const* fmt, ...); // C-style vararg
    static void stack_dump(lua_State* L);
    static bool run_script(lua_State* L, char const* filename);
    static bool eval(lua_State* L, char const* script);

///////////////////////////////////////////////////////////////////////

    static inline void push_(lua_State* L, bool const& v)   { lua_pushboolean(L, v); }
    static inline void push_(lua_State* L, double const& v) { lua_pushnumber(L, v); }
    static inline void push_(lua_State* L, int const& v)    { lua_pushinteger(L, v); }
    static inline void push_(lua_State* L, char const*& v)  { lua_pushstring(L, v); }
    template<int N>
    static inline void push_(lua_State* L, char const (&v)[N]) { lua_pushlstring(L, v, N); }
    static inline void push_(lua_State* L, void* const& v)     { lua_pushlightuserdata(L, v); }
    template<typename T>
    static inline void push_(lua_State* L, T const& v);/* { //not supported.
        printf("Lua: YOU CANNNNNNNNNOT PASSSSSSSS!\n");
        exit(EXIT_FAILURE);
    }*/

    template<typename T>
    static inline T fetch_(lua_State* L, int n);

#ifdef __SHOOTING_CUBES_CPP0X__
    static inline void push_args_(){}

    template<typename Head, typename... Tail>
    static inline void push_args_(lua_State* L, Head const& h, Tail const&... t)
    {
        push_(L, h);
        push_args_(L, t...);
    }

    template<int N, int I, typename Head, typename... Tail, typename ReturnObject>
    static inline void get_results_(lua_State* L, ReturnObject& res)
    {
        using namespace std::tr1;
        get<I>(res) = fetch_<Head>(L, -N);
        get_results_<N-1, I+1, Tail...>(L, res);
    }

    template<int N, int I, typename ReturnObject>
    static inline void get_results_(lua_State* L, ReturnObject& res){}

    template<typename... Rets, typename... Args>
    static std::tr1::tuple<Rets...>
    call_R(lua_State* L, char const* funcname, Args const&... args)
    {
        using namespace std::tr1;

        lua_getglobal(L, funcname); //tell lua to push the function into stack
        int const nargs = sizeof...(args), nrets = sizeof...(Rets);
        luaL_checkstack(L, nargs, "Too many arguments.");

        push_args_(L, args...);

        if( lua_pcall(L, nargs, nrets, 0) )
            error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));

        tuple<Rets...> results;
        get_results_<sizeof...(Rets), 0, Rets...>(L, results);
        return results;
    }

    template<typename... Args>
    static void call(lua_State* L, char const* funcname, Args const&... args)
    {
        lua_getglobal(L, funcname); //tell lua to push the function into stack
        int const nargs = sizeof...(args);
        luaL_checkstack(L, nargs, "Too many arguments.");

        push_args_(L, args...);

        if( lua_pcall(L, nargs, 0, 0) )
            error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));
    }

#else // __SHOOTING_CUBES_CPP0X__

    template<typename Ret> // Return 1 value with no args
    static Ret call_R(lua_State* L, char const* funcname)
    {
        lua_getglobal(L, funcname); //tell lua to push the function into stack

        //push_(L); // pushs nothing, no calling args.

        if( lua_pcall(L, 0, 1, 0) )
            error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));

        return fetch_<Ret>(L, -1); //support for 1 primitive lua type now.
    }

    template<typename Ret, typename T1> // Return 1 value with 1 args
    static Ret call_R(lua_State* L, char const* funcname, T1 a1)
    {
        lua_getglobal(L, funcname); //tell lua to push the function into stack

        push_(L, a1); //arg 1

        if( lua_pcall(L, 1, 1, 0) )
            error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));

        return fetch_<Ret>(L, -1); //support for 1 primitive lua type now.
    }

    template<typename Ret, typename T1, typename T2> // Return 1 value with 2 args
    static Ret call_R(lua_State* L, char const* funcname, T1 a1, T2 a2)
    {
        lua_getglobal(L, funcname); //tell lua to push the function into stack

        push_(L, a1); //arg 1
        push_(L, a2); //arg 2

        if( lua_pcall(L, 2, 1, 0) )
            error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));

        return fetch_<Ret>(L, -1); //support for 1 primitive lua type now.
    }

    template<typename Ret, typename T1, typename T2, typename T3> // Return 1 value with 3 args
    static Ret call_R(lua_State* L, char const* funcname, T1 a1, T2 a2, T3 a3)
    {
        lua_getglobal(L, funcname); //tell lua to push the function into stack

        push_(L, a1); //arg 1
        push_(L, a2); //arg 2
        push_(L, a3); //arg 3

        if( lua_pcall(L, 3, 1, 0) )
            error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));

        return fetch_<Ret>(L, -1); //support for 1 primitive lua type now.
    }

    template<typename Ret, typename T1, typename T2, typename T3, typename T4> // Return 1 value with 4 args
    static Ret call_R(lua_State* L, char const* funcname, T1 a1, T2 a2, T3 a3, T4 a4)
    {
        lua_getglobal(L, funcname); //tell lua to push the function into stack

        push_(L, a1); //arg 1
        push_(L, a2); //arg 2
        push_(L, a3); //arg 3
        push_(L, a4); //arg 4

        if( lua_pcall(L, 4, 1, 0) )
            error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));

        return fetch_<Ret>(L, -1); //support for 1 primitive lua type now.
    }

    template<typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5> // Return 1 value with 5 args
    static Ret call_R(lua_State* L, char const* funcname, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
    {
        lua_getglobal(L, funcname); //tell lua to push the function into stack

        push_(L, a1); //arg 1
        push_(L, a2); //arg 2
        push_(L, a3); //arg 3
        push_(L, a4); //arg 4
        push_(L, a5); //arg 5

        if( lua_pcall(L, 5, 1, 0) )
            error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));

        return fetch_<Ret>(L, -1); //support for 1 primitive lua type now.
    }

    // no return value with no args
    static void call(lua_State* L, char const* funcname)
    {
        lua_getglobal(L, funcname); //tell lua to push the function into stack

        //push_(L); // pushs nothing, no calling args.

        if( lua_pcall(L, 0, 0, 0) )
            error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));
    }

    template<typename T1> // no return value with 1 args
    static void call(lua_State* L, char const* funcname, T1 a1)
    {
        lua_getglobal(L, funcname); //tell lua to push the function into stack

        push_(L, a1); //arg 1

        if( lua_pcall(L, 1, 0, 0) )
            error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));
    }

    template<typename T1, typename T2> // no return value with 2 args
    static void call(lua_State* L, char const* funcname, T1 a1, T2 a2)
    {
        lua_getglobal(L, funcname); //tell lua to push the function into stack

        push_(L, a1); //arg 1
        push_(L, a2); //arg 2

        if( lua_pcall(L, 2, 0, 0) )
            error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));
    }

    template<typename T1, typename T2, typename T3> // no return value with 4 args
    static void call(lua_State* L, char const* funcname, T1 a1, T2 a2, T3 a3)
    {
        lua_getglobal(L, funcname); //tell lua to push the function into stack

        push_(L, a1); //arg 1
        push_(L, a2); //arg 2
        push_(L, a3); //arg 3

        if( lua_pcall(L, 3, 0, 0) )
            error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));
    }

    template<typename T1, typename T2, typename T3, typename T4> // no return value with 4 args
    static void call(lua_State* L, char const* funcname, T1 a1, T2 a2, T3 a3, T4 a4)
    {
        lua_getglobal(L, funcname); //tell lua to push the function into stack

        push_(L, a1); //arg 1
        push_(L, a2); //arg 2
        push_(L, a3); //arg 3
        push_(L, a4); //arg 4

        if( lua_pcall(L, 4, 0, 0) )
            error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5> // no return value with 5 args
    static void call(lua_State* L, char const* funcname, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
    {
        lua_getglobal(L, funcname); //tell lua to push the function into stack

        push_(L, a1); //arg 1
        push_(L, a2); //arg 2
        push_(L, a3); //arg 3
        push_(L, a4); //arg 4
        push_(L, a5); //arg 5

        if( lua_pcall(L, 5, 0, 0) )
            error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));
    }

#endif // __SHOOTING_CUBES_CPP0X__

///////////////////////////////////////////////////////////////////////////////

}; // struct Lua

template<> inline double      Lua::fetch_(lua_State* L, int n) { return luaL_checknumber(L, n); }
template<> inline    int      Lua::fetch_(lua_State* L, int n) { return luaL_checkint(L, n); }
template<> inline char const* Lua::fetch_(lua_State* L, int n) { return luaL_checkstring(L, n); }

} // script
} // psc

#endif// __SHOOTING_CUBES_LUA_UTILITY__
