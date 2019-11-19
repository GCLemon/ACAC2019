//////////////////////////////////////////////////
//
//    Player.hpp
//    自機のクラスを定義する
//

#pragma once

// Altseed
#include "../Altseed/Altseed.h"

using namespace std;
using namespace asd;

class Player : public TextureObject2D
{
private:

    // 当たり判定を行う時の半径
    float Radius;

public:

    // 当たり判定を行う時の半径を取得する
    float GetRadius() { return Radius; }

public:
    Player();

protected:
    void OnUpdate() override;

};