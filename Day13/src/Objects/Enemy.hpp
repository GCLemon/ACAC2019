//////////////////////////////////////////////////
//
//    Player.hpp
//    敵機のクラスを定義する
//

#pragma once

// Altseed
#include "../Altseed/Altseed.h"

// Lua
#include <lua/lua.hpp>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

using namespace std;
using namespace asd;

// Lua に渡すための関数
//==================================================
int Launch(lua_State*);
//==================================================

class Enemy : public TextureObject2D
{
private:
    // 敵機の動きを制御する Lua ステート
    lua_State* enemy_state;
    lua_State* enemy_movement;

public:
    Enemy(Vector2DF position);

protected:
    void OnUpdate() override;
};