//////////////////////////////////////////////////
//
//    program3-2.cpp
//    C++ で設定されたグローバル関数を Lua で使用する
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

    // 設定する値をスタックに積んで,積んだ値をグローバル変数に代入する
    lua_pushnumber(state, 56);
    lua_setglobal(state, "x");
    lua_pushnumber(state, 7);
    lua_setglobal(state, "y");

    // Lua スクリプトの実行
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