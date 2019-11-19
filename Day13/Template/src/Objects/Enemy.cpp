//////////////////////////////////////////////////
//
//    Enemy.cpp
//    敵機のクラスを実装する
//    ここに組み込みスクリプトを実装していく
//

#include "Enemy.hpp"
#include "EnemyBullet.hpp"
#include "PlayerBullet.hpp"

// Lua 側で弾を打つ関数
int Launch(lua_State* state)
{
    /*
    この関数は2つのテーブルを引数にとり
    第一引数時自身のテーブル(enemy),第二引数に弾を打つ方向(direction)が指定される.

    なお, Lua スクリプトで enemy テーブルは以下のように定義されているものとする.

    enemy = {

        position = {
            x = ...,
            y = ...
        },

        launch = function(this)
            ...
        end
    }
    */
}

Enemy::Enemy(Vector2DF position)
{
    // 敵機のテクスチャの設定
    auto texture = Engine::GetGraphics()->CreateTexture2D(u"resources/Enemy.png");
    SetTexture(texture);

    // オブジェクトの中心座標の設定
    Vector2DF texture_size = texture->GetSize().To2DF();
    SetCenterPosition(texture_size * 0.5f);

    // オブジェクトの描画位置の設定
    SetPosition(position);

    // 当たり判定の半径の設定
    Radius = GetTexture()->GetSize().X * 0.5f;
}

Enemy::~Enemy()
{
    // 使い終わった Lua ステートは必ず close する
    lua_close(EnemyState);
}

void Enemy::OnUpdate()
{
}