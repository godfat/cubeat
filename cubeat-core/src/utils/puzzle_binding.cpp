
#include "Conf.hpp"

#include "utils/lua_utility.hpp"
#include "utils/puzzle_binding.hpp"
#include <boost/tr1/tuple.hpp>
#include <cmath>
#include <dirent.h>
#include <errno.h>
#include <cstring>

using std::tr1::tuple;
using std::tr1::get;

inline void lua_push_(lua_State* L, bool const& v)   { lua_pushboolean(L, v); }
inline void lua_push_(lua_State* L, double const& v) { lua_pushnumber(L, v); }
inline void lua_push_(lua_State* L, int const& v)    { lua_pushinteger(L, v); }
inline void lua_push_(lua_State* L, char const*& v)  { lua_pushstring(L, v); }
template<int N>
inline void lua_push_(lua_State* L, char const (&v)[N]) {lua_pushlstring(L, v, N); }
template<typename T>
inline void lua_push_(lua_State* L, T const& v); //not implemented, so you know you can't push this.

template<typename T>
inline T lua_to_(lua_State* L, int n);
template<> inline double      lua_to_(lua_State* L, int n) { return lua_tonumber(L, n); }
template<> inline    int      lua_to_(lua_State* L, int n) { return lua_tointeger(L, n); }
template<> inline char const* lua_to_(lua_State* L, int n) { return lua_tostring(L, n); }

#ifdef __SHOOTING_CUBES_CPP0X__
inline void push_args_to_stack_(lua_State* L){}

template<typename Head, typename... Tail>
inline void push_args_to_stack_(lua_State* L, Head const& h, Tail const&... t)
{
    lua_push_(L, h);
    push_args_to_stack_(L, t...);
}

template<int N, int I, typename Head, typename... Tail, typename ReturnObject>
inline void get_results_(lua_State* L, ReturnObject& res)
{
    get<I>(res) = lua_to_<Head>(L, -N);
    get_results_<N-1, I+1, Tail...>(L, res);
}

template<int N, int I, typename ReturnObject>
inline void get_results_(lua_State* L, ReturnObject& res){}

template<typename... Rets, typename... Args>
tuple<Rets...>
call_lua_function_R(lua_State* L, char const* funcname, Args const&... args)
{
    lua_getglobal(L, funcname); //tell lua to push the function into stack
    int const nargs = sizeof...(args), nrets = sizeof...(Rets);
    luaL_checkstack(L, nargs, "Too many arguments.");

    push_args_to_stack_(L, args...);

    if( lua_pcall(L, nargs, nrets, 0) )
        error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));

    tuple<Rets...> results;
    get_results_<sizeof...(Rets), 0, Rets...>(L, results);
    return results;
}

template<typename... Args>
void call_lua_function(lua_State* L, char const* funcname, Args const&... args)
{
    lua_getglobal(L, funcname); //tell lua to push the function into stack
    int const nargs = sizeof...(args);
    luaL_checkstack(L, nargs, "Too many arguments.");

    push_args_to_stack_(L, args...);

    if( lua_pcall(L, nargs, 0, 0) )
        error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));
}

#else // __SHOOTING_CUBES_CPP0X__

template<typename Ret> // Return 1 value with no args
Ret call_lua_function(lua_State* L, char const* funcname)
{
    lua_getglobal(L, funcname); //tell lua to push the function into stack

    //lua_push_(L); // pushs nothing, no calling args.

    if( lua_pcall(L, 0, 1, 0) )
        error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));

    return lua_to_<Ret>(L, -1); //support for 1 primitive lua type now.
}


template<typename Ret, typename T1> // Return 1 value with 1 args
Ret call_lua_function(lua_State* L, char const* funcname, T1 a1)
{
    lua_getglobal(L, funcname); //tell lua to push the function into stack

    lua_push_(L, a1); //arg 1

    if( lua_pcall(L, 1, 1, 0) )
        error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));

    return lua_to_<Ret>(L, -1); //support for 1 primitive lua type now.
}

template<typename Ret, typename T1, typename T2> // Return 1 value with 2 args
Ret call_lua_function(lua_State* L, char const* funcname, T1 a1, T2 a2)
{
    lua_getglobal(L, funcname); //tell lua to push the function into stack

    lua_push_(L, a1); //arg 1
    lua_push_(L, a2); //arg 2

    if( lua_pcall(L, 2, 1, 0) )
        error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));

    return lua_to_<Ret>(L, -1); //support for 1 primitive lua type now.
}

template<typename Ret, typename T1, typename T2, typename T3> // Return 1 value with 3 args
Ret call_lua_function(lua_State* L, char const* funcname, T1 a1, T2 a2, T3 a3)
{
    lua_getglobal(L, funcname); //tell lua to push the function into stack

    lua_push_(L, a1); //arg 1
    lua_push_(L, a2); //arg 2
    lua_push_(L, a3); //arg 3

    if( lua_pcall(L, 3, 1, 0) )
        error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));

    return lua_to_<Ret>(L, -1); //support for 1 primitive lua type now.
}

template<typename Ret, typename T1, typename T2, typename T3, typename T4> // Return 1 value with 4 args
Ret call_lua_function(lua_State* L, char const* funcname, T1 a1, T2 a2, T3 a3, T4 a4)
{
    lua_getglobal(L, funcname); //tell lua to push the function into stack

    lua_push_(L, a1); //arg 1
    lua_push_(L, a2); //arg 2
    lua_push_(L, a3); //arg 3
    lua_push_(L, a4); //arg 4

    if( lua_pcall(L, 4, 1, 0) )
        error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));

    return lua_to_<Ret>(L, -1); //support for 1 primitive lua type now.
}

template<typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5> // Return 1 value with 5 args
Ret call_lua_function(lua_State* L, char const* funcname, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
{
    lua_getglobal(L, funcname); //tell lua to push the function into stack

    lua_push_(L, a1); //arg 1
    lua_push_(L, a2); //arg 2
    lua_push_(L, a3); //arg 3
    lua_push_(L, a4); //arg 4
    lua_push_(L, a5); //arg 5

    if( lua_pcall(L, 5, 1, 0) )
        error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));

    return lua_to_<Ret>(L, -1); //support for 1 primitive lua type now.
}

// no return value with no args
void call_lua_function(lua_State* L, char const* funcname)
{
    lua_getglobal(L, funcname); //tell lua to push the function into stack

    //lua_push_(L); // pushs nothing, no calling args.

    if( lua_pcall(L, 0, 0, 0) )
        error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));
}


template<typename T1> // no return value with 1 args
void call_lua_function(lua_State* L, char const* funcname, T1 a1)
{
    lua_getglobal(L, funcname); //tell lua to push the function into stack

    lua_push_(L, a1); //arg 1

    if( lua_pcall(L, 1, 0, 0) )
        error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));
}

template<typename T1, typename T2> // no return value with 2 args
void call_lua_function(lua_State* L, char const* funcname, T1 a1, T2 a2)
{
    lua_getglobal(L, funcname); //tell lua to push the function into stack

    lua_push_(L, a1); //arg 1
    lua_push_(L, a2); //arg 2

    if( lua_pcall(L, 2, 0, 0) )
        error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));
}

template<typename T1, typename T2, typename T3> // no return value with 4 args
void call_lua_function(lua_State* L, char const* funcname, T1 a1, T2 a2, T3 a3)
{
    lua_getglobal(L, funcname); //tell lua to push the function into stack

    lua_push_(L, a1); //arg 1
    lua_push_(L, a2); //arg 2
    lua_push_(L, a3); //arg 3

    if( lua_pcall(L, 3, 0, 0) )
        error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));
}

template<typename T1, typename T2, typename T3, typename T4> // no return value with 4 args
void call_lua_function(lua_State* L, char const* funcname, T1 a1, T2 a2, T3 a3, T4 a4)
{
    lua_getglobal(L, funcname); //tell lua to push the function into stack

    lua_push_(L, a1); //arg 1
    lua_push_(L, a2); //arg 2
    lua_push_(L, a3); //arg 3
    lua_push_(L, a4); //arg 4

    if( lua_pcall(L, 4, 0, 0) )
        error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));
}

template<typename T1, typename T2, typename T3, typename T4, typename T5> // no return value with 5 args
void call_lua_function(lua_State* L, char const* funcname, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
{
    lua_getglobal(L, funcname); //tell lua to push the function into stack

    lua_push_(L, a1); //arg 1
    lua_push_(L, a2); //arg 2
    lua_push_(L, a3); //arg 3
    lua_push_(L, a4); //arg 4
    lua_push_(L, a5); //arg 5

    if( lua_pcall(L, 5, 0, 0) )
        error(L, "error calling '%s': %s", funcname, lua_tostring(L, -1));
}

#endif // __SHOOTING_CUBES_CPP0X__

///////////////////////////////////////////////////////////////////////////////

bool load_lua_script(lua_State* L, char const* filename)
{
    if( luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0) ) {
        //bad call will return non-zero
        error(L, "cannot load file: \n  %s", lua_tostring(L, -1) );
        return true;
    }
    return false;
}

//int l_sin(lua_State* L)
//{
//    double in = luaL_checknumber(L, 1);
//    lua_pushnumber(L, std::sin(in));
//    return 1;
//}
//
//int l_dir(lua_State* L)
//{
//    DIR *dir;
//    struct dirent *entry;
//    char const* path = luaL_checkstring(L, 1); //get input from lua
//
//    /* open directory */
//    dir = opendir(path);
//    if( dir == NULL ) { //error
//        lua_pushnil(L);
//        lua_pushstring(L, strerror(errno)); // where does errno come from?
//        return 2; // 2 results
//    }
//
//    /* create result table */
//    lua_newtable(L);
//    int i = 0;
//    while( (entry = readdir(dir)) != NULL ) {
//        lua_pushnumber(L, ++i);
//        lua_pushstring(L, entry->d_name);
//        lua_settable(L, -3);  //add above entry into this table.
//    }
//    closedir(dir);
//    return 1; /* return the table (which is already at the top of the stack) */
//}
//
//int l_listmap(lua_State* L) //not practical when JIT'ed
//{
//    luaL_checktype(L, 1, LUA_TTABLE);    //first argument from lua should be table
//    luaL_checktype(L, 2, LUA_TFUNCTION); //second arg from lua should be function
//    int n = lua_objlen(L, 1); // get table(array)'s length
//    for( int i = 1; i <= n; ++i ) {
//        lua_pushvalue(L, 2);  // dupe f and push on top
//        lua_rawgeti(L, 1, i); // get t[i] from lua and push on top
//        lua_call(L, 1, 1);    // f(t[i])  -- this will consume two element from top, and push 1 result on top
//        lua_rawseti(L, 1, i); // pop top(result) and assign it to t[i]
//    }
//    //we don't need to pop stack here since the function stack is unique and will be collected.
//    return 0; // this call to C doesn't have any result
//}
//
//int l_split(lua_State* L) //not practical when JIT'ed
//{
//    char const* s = luaL_checkstring(L, 1);
//    char const* sep = luaL_checkstring(L, 2);
//    char const* e = 0;
//    int i = 1;
//
//    lua_newtable(L); // create the result to be passed back to lua
//
//    while( (e = strchr(s, *sep)) != NULL ) {
//        lua_pushlstring(L, s, e-s); //push substring
//        lua_rawseti(L, -2, i);
//        s = e + 1; ++i;
//    }
//    //push the last substring
//    lua_pushstring(L, s);
//    lua_rawseti(L, -2, i);
//    return 1;
//}
//
//const luaL_Reg mylib[] = { //plain C module
//    {"dir", &l_dir},
//    {"sine", &l_sin},
//    {"listmap", &l_listmap},
//    {"split", &l_split},
//    {NULL, NULL}
//};
//
//int luaopen_mylib(lua_State* L)
//{
//    luaL_register(L, "mylib", mylib);
//    return 1;
//}

int puzzle_binding(lua_State* L, int const& level)
{
/* Extending lua virtual machine right after we open a new lua VM */

//    lua_pushcfunction(L, &luaopen_mylib);
//    lua_setglobal(L, "luaopen_mylib");

/* Before we load or execute any script. */

    if( load_lua_script(L, psc::Conf::i().expand("script/puzzle/puzzle_gen_interface.lua").c_str() ) ) //will execute once globally
        return 1;

    call_lua_function(L, "generate_to_file", level, 6, 10, true); //temp: true for debug

    //try to get some table here...
//    lua_getglobal(L, "config");
//    lua_getfield(L, -1, "key"); //tell lua to push config["key"] to the stack
//
//    //now we retrieve it.
//    double ck = luaL_checknumber(L, -1);
//    lua_pop(L, 1); //pop the retrieved result
//    printf("config.key = %lf\n", ck);
//
//    tuple<int> r = call_lua_function<int>(L, "method1", 3, 4);
//    printf("%d\n", get<0>(r));
//
//    tuple<int, double, char const*> r2 =
//        call_lua_function<int, double, char const*>(L, "identity", 1, 2.0, "3");
//    printf("%d, %lf, %s\n", get<0>(r2), get<1>(r2), get<2>(r2));
//
//    tuple<double> r3= call_lua_function<double>(L, "call_c", 2.0, 1.14);
//    printf("%lf\n", get<0>(r3));
//
//    lua_settop(L, 0);
    return 0;
}
