//////////////////////////////////////////////////
//
//    program5-2.cpp
//    C++ で設定されたテーブルを Lua で使用する
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

    // Lua ファイルを読み込み
    luaL_loadfile(state, argv[1]);

    // テーブルの作成
    lua_newtable(state);
    lua_pushstring(state, "This is a lua script.");
    lua_setfield(state, -2, "string");
    lua_pushnumber(state, 3.14159265);
    lua_setfield(state, -2, "number");
    lua_pushboolean(state, true);
    lua_setfield(state, -2, "boolean");
    lua_setglobal(state, "table");

    // スクリプトの実行
    if(lua_pcall(state, 0, 0, 0))
    {
        // エラーを出力して終了
        printf("%s\n", lua_tostring(state, -1));
        lua_close(state);
        return -1;
    }

    // Lua ステートの破棄
    lua_close(state);

    return 0;
}