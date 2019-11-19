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

#include "PlayerBullet.hpp"

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

    // 当たり判定を行う時の半径
    float Radius;

public:

    // 当たり判定を行う時の半径を取得する
    float GetRadius() { return Radius; }

public:
    Enemy(Vector2DF position);
    ~Enemy();

protected:
    void OnUpdate() override;

};