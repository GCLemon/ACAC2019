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
public:
    Player();

protected:
    void OnUpdate() override;
};