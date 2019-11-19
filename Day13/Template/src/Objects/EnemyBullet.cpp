//////////////////////////////////////////////////
//
//    EnemyBullet.hpp
//    敵機の弾のクラスを実装する
//

#include "EnemyBullet.hpp"
#include "Player.hpp"

EnemyBullet::EnemyBullet(Vector2DF position, Vector2DF velocity)
{
    // 弾のテクスチャの設定
    auto texture = Engine::GetGraphics()->CreateTexture2D(u"resources/EnemyBullet.png");
    SetTexture(texture);

    // オブジェクトの中心座標の設定
    Vector2DF texture_size = texture->GetSize().To2DF();
    SetCenterPosition(texture_size * 0.5f);

    // オブジェクトの描画位置の設定
    SetPosition(position);

    // 当たり判定の半径の設定
    Radius = GetTexture()->GetSize().X * 0.5f;

    // 敵弾の速度の設定
    Velocity = velocity;
}

void EnemyBullet::OnUpdate()
{
    // 当たり判定を行う.
    bool is_shoot = false;
    for(auto obj : GetLayer()->GetObjects())
    {
        auto player = dynamic_pointer_cast<Player>(obj);
        if(player != nullptr)
        {
            Vector2DF player_pos = player->GetPosition();
            Vector2DF bullet_pos = GetPosition();
            float distance = (player_pos - bullet_pos).GetLength();
            if(distance < player->GetRadius() + GetRadius())
            {
                player->Dispose();
                is_shoot = true;
            }
        }
    }
    if(is_shoot) Dispose();

    // 描画位置の設定
    Vector2DF position = GetPosition();
    position += Velocity;
    SetPosition(position);

    // 画面外に飛んだら Dispose
    Vector2DF texture_size = GetTexture()->GetSize().To2DF();
    Vector2DF window_size  = Engine::GetWindowSize().To2DF();
    if(position.X < -texture_size.X || window_size.X + texture_size.X < position.X
    || position.Y < -texture_size.Y || window_size.Y + texture_size.Y < position.Y)
        Dispose();
}