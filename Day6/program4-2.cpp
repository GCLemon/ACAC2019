//////////////////////////////////////////////////
//
//    program4-2.cpp
//    C++ で定義された関数を Lua で使用する
//

#include <cmath>

#include <lua/lua.hpp>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

using namespace std;

// Lua が理解できるような形で関数を定義する
// Lua の組み込みにおいて, lua_State* を引数にとり,
// Lua スクリプトでの戻り値の数を返す↓この関数を「グルー(Glue)関数」と呼ぶ 
int arithmetic(lua_State* state)
{
    // スタックから引数を受け取る
    double x = lua_tonumber(state, -2);
    double y = lua_tonumber(state, -1);

    // スタックの削除
    lua_pop(state, -1);

    // 和・差・積・商を戻り値としてスタックに積む
    lua_pushnumber(state, x + y);
    lua_pushnumber(state, x - y);
    lua_pushnumber(state, x * y);
    lua_pushnumber(state, x / y);

    // 最後に戻り値の数を返す
    return 4;
}

int main(int argc, char** argv)
{
    // Lua ステートの作成
    lua_State* state = luaL_newstate();
    luaL_openlibs(state);

    // Lua ファイルの読み込み
    luaL_loadfile(state, argv[1]);

    // C++ で定義された関数をスタックに積み,
    // 積んだ関数をグローバル変数として設定する
    lua_pushcfunction(state, arithmetic);
    lua_setglobal(state, "arithmetic");
    
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