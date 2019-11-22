//////////////////////////////////////////////////
//
//    program3-1.cpp
//    Lua で定義されたグローバル変数を C++ で取得する
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

    // Lua スクリプトの実行
    if(lua_pcall(state, 0, 0, 0))
    {
        // エラーを出力して終了
        printf("%s\n", lua_tostring(state, -1));
        lua_close(state);
        return -1;
    }

    // グローバル変数を読み出してプリント
    lua_getglobal(state, "string");
    printf("string = %s\n", lua_tostring(state, -1));
    lua_getglobal(state, "number");
    printf("number = %f\n", lua_tonumber(state, -1));
    lua_getglobal(state, "boolean");
    printf("boolean = %s\n", lua_toboolean(state, -1) ? "true" : "false");

    // Lua ステートの破棄
    lua_close(state);

    return 0;
}