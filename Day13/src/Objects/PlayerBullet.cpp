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
}

void PlayerBullet::OnUpdate()
{
    // 描画位置の設定
    Vector2DF position = GetPosition();
    position += Vector2DF(0, -20);
    SetPosition(position);

    // 画面外に飛んだら Dispose
    if(position.Y < -GetTexture()->GetSize().Y)
        Dispose();
}