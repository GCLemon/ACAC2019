//////////////////////////////////////////////////
//
//    GameScene.hpp
//    ゲームのシーンを定義する
//

#pragma once

// Altseed
#include "../Altseed/Altseed.h"

// Lua
#include <lua/lua.hpp>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

#include "../Objects/Player.hpp"
#include "../Objects/Enemy.hpp"

using namespace std;
using namespace asd;

// Lua に渡すための関数
//==================================================
int AddEnemy(lua_State*);
//==================================================

class GameScene : public Scene
{
private:

    // ステージを制御する Lua ステート
    lua_State* StageState;
    lua_State* StageMovement;

    // このシーンに登録されているレイヤー
    shared_ptr<Layer2D> GameLayer;

public:
    GameScene();
    ~GameScene();

protected:
    void OnRegistered() override;
    void OnUpdated() override;
};