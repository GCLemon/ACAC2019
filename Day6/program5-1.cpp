//////////////////////////////////////////////////
//
//    program5-1.cpp
//    Lua で定義されたテーブルの内容を C++ で読み出す
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

    // Lua ファイルを読み込んで実行
    luaL_loadfile(state, argv[1]);
    if(lua_pcall(state, 0, 0, 0))
    {
        // エラーを出力して終了
        printf("%s\n", lua_tostring(state, -1));
        lua_close(state);
        return -1;
    }

    // 読み込んだファイルからテーブルの情報を読みだす
    lua_getglobal(state, "table");
    int table_pos = lua_gettop(state);

    // テーブルに格納された変数の情報を読み出す
    lua_getfield(state, table_pos, "number");
    printf("number = %lf\n", lua_tonumber(state, -1));
    lua_getfield(state, table_pos, "string");
    printf("string = %s\n", lua_tostring(state, -1));
    lua_getfield(state, table_pos, "boolean");
    printf("boolean = %s\n", lua_toboolean(state, -1) ? "true" : "false");

    // Lua ステートの破棄
    lua_close(state);

    return 0;
}