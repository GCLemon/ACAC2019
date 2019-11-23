//////////////////////////////////////////////////
//
//    Program2-2.cs
//    プログラム側で設定されたグローバル変数を
//    スクリプト側で使用する
//

using System;
using System.IO;

using Microsoft.CodeAnalysis.Scripting;
using Microsoft.CodeAnalysis.CSharp.Scripting;

namespace Roslyn
{
    public class Program2_2
    {
        // スクリプトに渡すグローバル変数
        public class GlobalParams
        {
            public int X;
            public int Y;
        }

        public static void Run()
        {
            // スクリプトの実行時オプション
            var option = ScriptOptions.Default.WithImports("System");

            try
            {
                // ファイルからスクリプトを作成
                var stream = File.OpenRead("Scripts/Script2-2.csx");
                var script = CSharpScript.Create(stream, option, typeof(GlobalParams));


                // スクリプト実行
                script.RunAsync(new GlobalParams { X = 56, Y = 7 });
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
