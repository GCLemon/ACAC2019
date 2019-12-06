//////////////////////////////////////////////////
//
//    PlayerBullet.hpp
//    自機の弾のクラスを定義する
//

#pragma once

// Altseed
#include "../Altseed/Altseed.h"

#include "Enemy.hpp"

using namespace std;
using namespace asd;

class PlayerBullet : public TextureObject2D
{
    friend class Enemy;

private:

    // 当たり判定を行う時の半径
    float Radius;

public:

    // 当たり判定を行う時の半径を取得する
    float GetRadius() { return Radius; }

public:
    PlayerBullet(Vector2DF position);

protected:
    void OnUpdate() override;

};