//////////////////////////////////////////////////
//
//    program6.cpp
//    コルーチンを使う
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

    // スレッドを生成する
    lua_State* coroutine = lua_newthread(state);

    // コルーチンステート内にあるコルーチン関数を指定する
    lua_getglobal(coroutine, "co");
    
    // コルーチンの実行が終わるまでループ
    while(lua_resume(coroutine, nullptr, 0))
        printf("%s\n", lua_tostring(coroutine, -1));

    // Lua ステートの破棄
    lua_close(state);

    return 0;
}