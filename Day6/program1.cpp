//////////////////////////////////////////////////
//
//    program1.cpp
//    Lua スクリプトを丸々実行する
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
    lua_pcall(state, 0, 0, 0);

    // スクリプト実行時に発生したエラーに対して処理をする場合,
    // 以下のように書き換える
    /*
    if(lua_pcall(state, 0, 0, 0))
    {
        // エラーを出力して終了
        printf("%s\n", lua_tostring(state, -1));
        lua_close(state);
        return -1;
    }
    */

    // Lua ステートの破棄
    lua_close(state);

    return 0;
}