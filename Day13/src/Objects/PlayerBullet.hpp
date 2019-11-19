//////////////////////////////////////////////////
//
//    PlayerBullet.hpp
//    自機の弾のクラスを定義する
//

#pragma once

// Altseed
#include "../Altseed/Altseed.h"

using namespace std;
using namespace asd;

class PlayerBullet : public TextureObject2D
{
public:
    PlayerBullet(Vector2DF position);

protected:
    void OnUpdate() override;
};