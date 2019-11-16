//////////////////////////////////////////////////
//
//    Altseed で制作したゲームに Lua スクリプトを
//    組み込むサンプル
//

// Lua
#include <lua/lua.hpp>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

// Altseed
#include "Altseed.h"

using namespace asd;
using namespace std;

//////////////////////////////////////////////////
//
//    敵機クラスの定義
//
class Enemy : public TextureObject2D
{
private:
    lua_State* enemy_state;
    lua_State* enemy_movement;

protected:
    void OnUpdate() override;

public:
    Enemy();
    ~Enemy();
};

//////////////////////////////////////////////////
//
//    敵機クラスの実装
//

// コンストラクタ
Enemy::Enemy() : TextureObject2D()
{
    // 基本情報の設定
    SetTexture(Engine::GetGraphics()->CreateTexture2D(u"Enemy.png"));
    SetCenterPosition(Vector2DF(15, 15));
    SetPosition(Vector2DF(320, 240));

    // 敵機に埋め込む Lua ステートを作成し,ファイルを読み込み
    enemy_state = luaL_newstate();
    luaL_openlibs(enemy_state);
    if(luaL_dofile(enemy_state, "Enemy.lua"))
    {
        // エラーメッセージを出力して強制終了
        printf("%s\n", lua_tostring(enemy_state, -1));
        lua_close(enemy_state);
        exit(0);
    }

    // コルーチンスレッドの生成
    enemy_movement = lua_newthread(enemy_state);
    lua_getglobal(enemy_movement, "enemy_movement");
}

// デストラクタ
Enemy::~Enemy()
{
    // Lua ステートのクローズ
    lua_close(enemy_state);
}

// 更新処理
void Enemy::OnUpdate()
{
    // テーブルの作成
    lua_newtable(enemy_movement);
    lua_pushnumber(enemy_movement, GetPosition().X);
    lua_setfield(enemy_movement, -2, "x");
    lua_pushnumber(enemy_movement, GetPosition().Y);
    lua_setfield(enemy_movement, -2, "y");
    lua_setglobal(enemy_movement, "position");
    
    // コルーチンの実行
    lua_resume(enemy_movement, nullptr, 0);

    // テーブル情報の読み出し
    lua_getglobal(enemy_movement, "position");
    int table_pos = lua_gettop(enemy_movement);
    lua_getfield(enemy_movement, table_pos, "x");
    lua_getfield(enemy_movement, table_pos, "y");

    // ポジションの設定
    SetPosition(Vector2DF(
        lua_tonumber(enemy_movement, -2),
        lua_tonumber(enemy_movement, -1)
    ));
}

//////////////////////////////////////////////////
//
//    メイン関数
//

int main()
{
    // Altseed の初期化
    Engine::Initialize(u"Lua Scripting Sample", 640, 480, EngineOption());

    // シーン・レイヤー・敵機オブジェクトの生成
    shared_ptr<Scene>   scene_ptr = make_shared<Scene>();
    shared_ptr<Layer2D> layer_ptr = make_shared<Layer2D>();
    shared_ptr<Enemy>   enemy_ptr = make_shared<Enemy>();

    // シーン・レイヤー・敵機オブジェクトの登録
    Engine::ChangeScene(scene_ptr);
    scene_ptr->AddLayer(layer_ptr);
    layer_ptr->AddObject(enemy_ptr);

    // Altseed の更新処理
    while(Engine::DoEvents())
    {
        Engine::Update();
        if(Engine::GetKeyboard()->GetKeyState(Keys::Escape) == ButtonState::Push)
        {
            break;
        }
    }

    // Altseed の終了処理
    Engine::Terminate();
}