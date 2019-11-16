//////////////////////////////////////////////////
//
//    program4-1.cpp
//    Lua で定義された関数を C++ で使用する
//

#include <lua/lua.hpp>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

using namespace std;

int main(int argc, char** argv)
{
    // Lua ステートの作成
    lua_State* state = luaL_newstate();
    luaL_openlibs(state);

    // Lua ファイルの読み込み
    luaL_loadfile(state, argv[1]);

    // まずは Lua スクリプトを引数なしで実行
    if(lua_pcall(state, 0, 0, 0))
    {
        // エラーを出力して終了
        printf("%s\n", lua_tostring(state, -1));
        lua_close(state);
        return -1;
    }

    // Lua スクリプトで定義された関数をスタックに積む
    lua_getglobal(state, "arithmetic");

    // 引数とする値をスタックに積む
    lua_pushnumber(state, 64);
    lua_pushnumber(state, 36);

    // 実行する関数をスタックのトップに持ってきた上で
    // もう一度スクリプトを実行する
    if(lua_pcall(state, 2, 4, 0))
    {
        // エラーを出力して終了
        printf("%s\n", lua_tostring(state, -1));
        lua_close(state);
        return -1;
    }

    // 戻り値は Lua スクリプトで return された順にスタックに積まれる
    printf("sum = %lf\n", lua_tonumber(state, -4));
    printf("dif = %lf\n", lua_tonumber(state, -3));
    printf("mul = %lf\n", lua_tonumber(state, -2));
    printf("div = %lf\n", lua_tonumber(state, -1));

    // Lua ステートの破棄
    lua_close(state);

    return 0;
}