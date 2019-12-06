//////////////////////////////////////////////////
//
//    Player.cpp
//    自機のクラスを実装する
//

#include "Player.hpp"
#include "PlayerBullet.hpp"

Player::Player()
{
    // プレイヤーのテクスチャの設定
    auto texture = Engine::GetGraphics()->CreateTexture2D(u"Resources/Player.png");
    SetTexture(texture);

    // オブジェクトの中心座標の設定
    Vector2DF texture_size = texture->GetSize().To2DF();
    SetCenterPosition(texture_size * 0.5f);

    // オブジェクトの描画位置の設定
    SetPosition(Vector2DF(320, 360));

    // 当たり判定の半径の設定
    Radius = GetTexture()->GetSize().X * 0.125f;
}

void Player::OnUpdate()
{
    // 描画位置の取得
    Vector2DF position = GetPosition();

    // 左に移動
    if(Engine::GetKeyboard()->GetKeyState(Keys::Left) == ButtonState::Hold)
        position += Vector2DF(-5, 0);

    // 右に移動
    if(Engine::GetKeyboard()->GetKeyState(Keys::Right) == ButtonState::Hold)
        position += Vector2DF(5, 0);

    // 下に移動
    if(Engine::GetKeyboard()->GetKeyState(Keys::Up) == ButtonState::Hold)
        position += Vector2DF(0, -5);

    // 上に移動
    if(Engine::GetKeyboard()->GetKeyState(Keys::Down) == ButtonState::Hold)
        position += Vector2DF(0, 5);

    // 自機の描画位置の調整
    Vector2DF texture_size = GetTexture()->GetSize().To2DF();
    Vector2DF window_size  = Engine::GetWindowSize().To2DF();
    position.X = clamp(position.X, texture_size.X * 0.5f, window_size.X - texture_size.X * 0.5f);
    position.Y = clamp(position.Y, texture_size.Y * 0.5f, window_size.Y - texture_size.Y * 0.5f);

    // 描画位置の設定
    SetPosition(position);

    // 弾を撃つ
    if(Engine::GetKeyboard()->GetKeyState(Keys::Z) == ButtonState::Push)
        GetLayer()->AddObject(make_shared<PlayerBullet>(position + Vector2DF(0, -30)));
}