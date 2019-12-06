using System;
using KeraLua;

namespace KeraLuaSample
{
    // スクリプティング可能なオブジェクトのサンプル
    public class ScriptableObject
    {
        // 内部状態の制御に用いるLuaステート
        private readonly Lua State;
        private readonly Lua Coroutine;

        // 内部情報として平面上の座標をもつ
        private double Position_X;
        private double Position_Y;

        // コンストラクタ
        public ScriptableObject()
        {
            // 変数の初期化
            Position_X = 0;
            Position_Y = 0;

            // スクリプトの読み込み
            State = new Lua();
            State.OpenLibs();
            State.DoFile("scriptA.lua");

            // コルーチンの生成
            Coroutine = State.NewThread();
            Coroutine.GetGlobal("movement");

            // move関数を定義
            Coroutine.PushCFunction(Move);
            Coroutine.SetGlobal("move");
        }

        // オブジェクトが「動く」メソッド
        // Lua側で使用する
        private int Move(IntPtr int_ptr)
        {
            // ステートの取得
            Lua state = Lua.FromIntPtr(int_ptr);

            // 引数の取得
            double Delta_X = state.ToNumber(-2);
            double Delta_Y = state.ToNumber(-1);

            // 内部情報の変更
            Position_X += Delta_X;
            Position_Y += Delta_Y;

            // 一応ステートを空にしておく
            state.Pop(state.GetTop());

            // 戻り値はない
            return 0;
        }

        // オブジェクトを更新する
        public void Update()
        {
            // コルーチンの実行
            Coroutine.Resume(null, 0);
        }

        // 座標をプリントする
        public void PrintCoordinate()
        {
            // プリント
            Console.WriteLine("(" + Position_X + "," + Position_Y + ")");
        }
    }
}
