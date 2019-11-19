#include "EnemyBullet.hpp"

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

    // 擲弾の速度の設定
    Velocity = velocity;
}

void EnemyBullet::OnUpdate()
{
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