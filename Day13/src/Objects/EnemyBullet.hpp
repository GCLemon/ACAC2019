//////////////////////////////////////////////////
//
//    PlayerBullet.hpp
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
    Vector2DF Velocity;

public:
    EnemyBullet(Vector2DF position, Vector2DF velocity);

protected:
    void OnUpdate() override;
};