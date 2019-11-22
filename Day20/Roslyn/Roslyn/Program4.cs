//////////////////////////////////////////////////
//
//    Program4.cs
//    C# スクリプトのオブジェクト指向的(?)な使い方
//

using System;
using System.IO;

using Microsoft.CodeAnalysis.Scripting;
using Microsoft.CodeAnalysis.CSharp.Scripting;

namespace Roslyn
{
    public class Program4
    {
        // プレイヤークラス
        public class Player
        {
            // プレイヤーごとに保存するスクリプト
            private readonly Script PlayerScript;

            // プレイヤーの内部状態
            public int Experience;
            public int ExperienceThreshold;
            public int Level;

            public Player()
            {
                // 変数の初期化
                Experience = 0;
                ExperienceThreshold = 5;
                Level = 1;

                // スクリプトの実行時オプション
                var option = ScriptOptions.Default.WithImports("System");

                // ファイルからスクリプトを作成
                var stream = File.OpenRead("Scripts/Script4.csx");
                PlayerScript = CSharpScript.Create(stream, option, typeof(Player));
            }

            // レベルアップする
            public void LevelUp()
            {
                // レベルをあげる
                ++Level;

                // 次のレベルアップ時の閾値を設定する
                ExperienceThreshold += 3 + Level * 2;
            }

            // Playerの更新処理
            public void Update()
            {
                // スクリプトを実行する
                PlayerScript.RunAsync(this);
            }
        }

        public static void Run()
        {
            try
            {
                // プレイヤーのインスタンスの作成
                var player = new Player();

                // プレイヤーの更新処理を30回繰り返す
                for (int i = 0; i < 30; ++i) player.Update();
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
