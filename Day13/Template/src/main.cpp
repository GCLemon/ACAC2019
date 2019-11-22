#include "Altseed/Altseed.h"

#include "Objects/Player.hpp"
#include "Objects/Enemy.hpp"

using namespace std;
using namespace asd;

int main(int argc, char** argv)
{
    // エンジンの初期化
    Engine::Initialize(u"Lua Scripting Sample", 640, 480, EngineOption());

    // オブジェクトを表示するためのシーンやレイヤーを用意
    shared_ptr<Scene> scene = make_shared<Scene>();
    shared_ptr<Layer2D> layer = make_shared<Layer2D>();

    // レイヤーに自機の追加
    shared_ptr<Player> player = make_shared<Player>();
    layer->AddObject(player);

    // レイヤーに敵機の追加
    shared_ptr<Enemy> enemy = make_shared<Enemy>(Vector2DF(100, 100));
    layer->AddObject(enemy);

    // シーンにレイヤーの登録
    layer->SetName(u"GameLayer");
    scene->AddLayer(layer);

    // シーンチェンジ
    Engine::ChangeScene(scene);

    // エンジンの更新処理
    while(Engine::DoEvents())
    {
        Engine::Update();

        // Escape キーが押されたらメインループを抜ける
        if(Engine::GetKeyboard()->GetKeyState(Keys::Escape) == ButtonState::Push)
        {
            break;
        }
    }

    // エンジンの終了宣言
    Engine::Terminate();
}