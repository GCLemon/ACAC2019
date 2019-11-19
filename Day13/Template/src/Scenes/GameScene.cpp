//////////////////////////////////////////////////
//
//    GameScene.hpp
//    ゲームのシーンを実装する
//

#include "GameScene.hpp"

int AddEnemy(lua_State* state)
{
    /*
    この関数はテーブルを引数にとり,敵の配置位置が指定される.

    なお,引数に指定するテーブルは以下のように定義されているものとする.

    position = {
        x = ...,
        y = ...
    }
    */
}

GameScene::GameScene()
{
    // オブジェクトを表示するためのレイヤーを用意・登録
    GameLayer = make_shared<Layer2D>();
    GameLayer->SetName(u"GameLayer");

    // レイヤーに自機の追加
    GameLayer->AddObject(make_shared<Player>());

    // ステージを制御するステートを作成し,スクリプトをファイルから読み込み
    StageState = luaL_newstate();
    luaL_openlibs(StageState);
    if(luaL_dofile(StageState, "scripts/Stage.lua"))
    {
        // エラーメッセージを出力して強制終了
        printf("%s\n", lua_tostring(StageState, -1));
        lua_close(StageState);
        exit(0);
    }

    // コルーチンを実行するためのスレッドを生成
    StageMovement = lua_newthread(StageState);
    lua_getglobal(StageMovement, "stage_movement");

    // 敵を追加する関数を Lua スクリプトに公開
    lua_pushcfunction(StageMovement, &AddEnemy);
    lua_setglobal(StageMovement, "add_enemy");
}

GameScene::~GameScene()
{
    // 使い終わった Lua ステートは必ず close する
    lua_close(StageState);
}

void GameScene::OnRegistered()
{
    AddLayer(GameLayer);
}

void GameScene::OnUpdated()
{
}