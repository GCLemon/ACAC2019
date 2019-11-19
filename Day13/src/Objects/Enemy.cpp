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

    // 受け取った2つのテーブルについてフィールドを展開する
    lua_getfield(state, -2, "position");
    lua_getfield(state, -1, "x");
    lua_getfield(state, -2, "y");
    lua_getfield(state, -4, "x");
    lua_getfield(state, -5, "y");

    // 受け取ったテーブルの情報から敵弾のインスタンスを作成する.
    shared_ptr<EnemyBullet> enemy_bullet = 
        make_shared<EnemyBullet>(
            Vector2DF(lua_tonumber(state, -4), lua_tonumber(state, -3)),
            Vector2DF(lua_tonumber(state, -2), lua_tonumber(state, -1))
        );

    // "GameLayer" という名前から敵弾を追加するレイヤーを探す
    shared_ptr<Layer2D> game_layer = nullptr;
    for(auto layer : Engine::GetCurrentScene()->GetLayers())
        if(layer->GetName() == u"GameLayer")
        {
            // Layer2Dにキャストできることを確認してから代入する.
            auto l = dynamic_pointer_cast<Layer2D>(layer);
            if(l != nullptr) game_layer = l;
        }

    // 敵弾を追加する
    game_layer->AddObject(enemy_bullet);

    // スタックを空にする
    lua_pop(state, lua_gettop(state));

    // 戻り値なし
    return 0;
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

    // 敵の動作を制御するステートを作成し,スクリプトをファイルから読み込み
    enemy_state = luaL_newstate();
    luaL_openlibs(enemy_state);
    if(luaL_dofile(enemy_state, "scripts/Enemy.lua"))
    {
        // エラーメッセージを出力して強制終了
        printf("%s\n", lua_tostring(enemy_state, -1));
        lua_close(enemy_state);
        exit(0);
    }

    // コルーチンを実行するためのスレッドを生成
    enemy_movement = lua_newthread(enemy_state);
    lua_getglobal(enemy_movement, "enemy_movement");

    // コルーチンステートにenemyテーブルを定義する
    // 敵機に関する情報はLua側で保存するものとする
    lua_newtable(enemy_movement);
    lua_newtable(enemy_movement);
    lua_pushnumber(enemy_movement, GetPosition().X);
    lua_setfield(enemy_movement, -2, "x");
    lua_pushnumber(enemy_movement, GetPosition().Y);
    lua_setfield(enemy_movement, -2, "y");
    lua_setfield(enemy_movement, -2, "position");
    lua_pushcfunction(enemy_movement, &Launch);
    lua_setfield(enemy_movement, -2, "launch");
    lua_setglobal(enemy_movement, "enemy");
}

Enemy::~Enemy()
{
    // 使い終わった Lua ステートは必ず close する
    lua_close(enemy_state);
}

void Enemy::OnUpdate()
{
    // コルーチンの実行
    lua_resume(enemy_movement, nullptr, 0);

    // Luaで保存されている敵機の情報を読み出し
    lua_getglobal(enemy_movement, "enemy");
    lua_getfield(enemy_movement, -1, "position");
    lua_getfield(enemy_movement, -1, "x");
    lua_getfield(enemy_movement, -2, "y");

    // ポジションの設定
    Vector2DF position = Vector2DF(
        (float)lua_tonumber(enemy_movement, -2),
        (float)lua_tonumber(enemy_movement, -1)
    );
    SetPosition(position);

    // コルーチン以外の要素をポップする
    lua_pop(enemy_movement, lua_gettop(enemy_movement) - 1);
}