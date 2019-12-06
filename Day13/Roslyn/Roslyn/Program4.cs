//////////////////////////////////////////////////
//
//    Program4.cs
//    C# スクリプトでインスタンスの内部状態を制御する
//

using System;
using System.IO;

using Microsoft.CodeAnalysis.Scripting;
using Microsoft.CodeAnalysis.CSharp.Scripting;

namespace Roslyn
{
    public class Program4
    {
        // 敵機のクラス
        public class Enemy
        {
            // 敵機ごとに保存するスクリプト
            private readonly Script PlayerScript;

            // 敵機の内部状態
            public int FrameCount;
            public double Position_X;
            public double Position_Y;

            public Enemy()
            {
                // 変数の初期化
                Position_X = 320;
                Position_Y = 160;

                // スクリプトの実行時オプション
                var option = ScriptOptions.Default.WithImports("System");

                // ファイルからスクリプトを作成
                var stream = File.OpenRead("Scripts/Script4.csx");
                PlayerScript = CSharpScript.Create(stream, option, typeof(Enemy));
            }

            // 敵機の更新処理
            public void Update()
            {
                // スクリプトを実行する
                PlayerScript.RunAsync(this);

                // 実行後の内部状態を出力
                Console.WriteLine("X = {0}, Y = {1}", Position_X, Position_Y);

                // フレーム数をカウント
                ++FrameCount;
            }
        }

        public static void Run()
        {
            try
            {
                // 敵機のインスタンスの作成
                var enemy = new Enemy();

                // 敵機の更新処理を30回繰り返す
                for (int i = 0; i < 30; ++i) enemy.Update();
            }

            // スクリプト実行時に発生した例外
            catch (CompilationErrorException e)
            {
                Console.WriteLine(e.Message);
            }

            // プログラム実行時に発生した例外
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }
    }
}
