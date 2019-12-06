//////////////////////////////////////////////////
//
//    PlayerBullet.cpp
//    自機の弾のクラスを実装する
//


#include "PlayerBullet.hpp"

PlayerBullet::PlayerBullet(Vector2DF position)
{
    // 弾のテクスチャの設定
    auto texture = Engine::GetGraphics()->CreateTexture2D(u"resources/PlayerBullet.png");
    SetTexture(texture);

    // オブジェクトの中心座標の設定
    Vector2DF texture_size = texture->GetSize().To2DF();
    SetCenterPosition(texture_size * 0.5f);

    // オブジェクトの描画位置の設定
    SetPosition(position);

    // 当たり判定の半径の設定
    Radius = GetTexture()->GetSize().X * 0.5f;
}

void PlayerBullet::OnUpdate()
{
    // 当たり判定を行う.
    bool is_shoot = false;
    for(auto obj : GetLayer()->GetObjects())
    {
        auto enemy = dynamic_pointer_cast<Enemy>(obj);
        if(enemy != nullptr)
        {
            Vector2DF enemy_pos = enemy->GetPosition();
            Vector2DF bullet_pos = GetPosition();
            float distance = (enemy_pos - bullet_pos).GetLength();
            if(distance < enemy->GetRadius() + GetRadius())
            {
                enemy->Dispose();
                is_shoot = true;
            }
        }
    }
    if(is_shoot) Dispose();

    // 描画位置の設定
    Vector2DF position = GetPosition();
    position += Vector2DF(0, -10);
    SetPosition(position);

    // 画面外に飛んだら Dispose
    if(position.Y < -GetTexture()->GetSize().Y)
        Dispose();
}