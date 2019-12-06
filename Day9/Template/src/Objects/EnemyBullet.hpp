//////////////////////////////////////////////////
//
//    EnemyBullet.hpp
//    敵機の弾のクラスを定義する
//

#pragma once

// Altseed
#include "../Altseed/Altseed.h"

using namespace std;
using namespace asd;

class EnemyBullet : public TextureObject2D
{
private:

    // 当たり判定を行う時の半径
    float Radius;

    // 弾が動く速度
    Vector2DF Velocity;

public:

    // 当たり判定を行う時の半径を取得する
    float GetRadius() { return Radius; }

public:
    EnemyBullet(Vector2DF position, Vector2DF velocity);

protected:
    void OnUpdate() override;

};