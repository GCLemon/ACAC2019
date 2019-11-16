//////////////////////////////////////////////////
//
//    program2.cpp
//    Lua ステートが持つスタックに値を push したり
//    pop したりする
//

#include <lua/lua.hpp>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

using namespace std;

// スタックの内容を表示する関数
void print_stack(lua_State* state)
{
    printf("\n==================================================\n");

    // 最後に格納した値のインデックスを取得・表示
    // このインデックスがスタックに積まれた値の数である.
    int stack_amount = lua_gettop(state);
    printf("%d values are in this stack.", stack_amount);

    printf("\n--------------------------------------------------\n");

    // スタックの内容の表示
    for(int i = stack_amount; i > 0; --i)
    {
        // 値が格納されているインデックスの表示
        printf("%03d(%04d) : ", i, i - stack_amount - 1);

        // 型名を取得して条件分岐
        switch(lua_type(state, i))
        {
        case LUA_TNIL:
            printf("NIL           : \n");
            break;
        case LUA_TBOOLEAN:
            // lua_toboolean でスタックの値を bool 型に変換
            printf("BOOLEAN       : %s\n", lua_toboolean(state, i) ? "true" : "false");
            break;
        case LUA_TLIGHTUSERDATA:
            printf("LIGHTUSERDATA : \n");
            break;
        case LUA_TNUMBER:
            // lua_tonumber でスタックの値を double 型に変換
            printf("NUMBER        : %f\n", lua_tonumber(state, i));
            break;
        case LUA_TSTRING:
            // lua_tostring でスタックの値を string 型に変換
            printf("STRING        : %s\n", lua_tostring(state, i));
            break;
        case LUA_TTABLE:
            printf("TABLE         : \n");
            break;
        case LUA_TFUNCTION:
            printf("FUNCTION      : \n");
            break;
        case LUA_TUSERDATA:
            printf("USERDATA      : \n");
            break;
        case LUA_TTHREAD:
            printf("THREAD        : \n");
            break;
        }
    }

    printf("==================================================\n");
}

int main(int argc, char** argv)
{
    // Lua ステートの作成
    lua_State* state = luaL_newstate();
    luaL_openlibs(state);

    // Lua ステートのスタックに値を積む
    lua_pushnil(state);
    lua_pushnumber(state, 2.71);
    lua_pushnumber(state, 3.14);
    lua_pushstring(state, "Lua");
    lua_pushboolean(state, true);

    // スタックの中身を出力
    print_stack(state);
    printf("\n");

    // Lua ステートの破棄
    lua_close(state);

    return 0;
}