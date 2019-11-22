//////////////////////////////////////////////////
//
//    Program3.cs
//    プログラム側で定義されたグローバル変数を
//    スクリプト側で使用する
//

using System;
using System.IO;

using Microsoft.CodeAnalysis.Scripting;
using Microsoft.CodeAnalysis.CSharp.Scripting;

namespace Roslyn
{
    public class Program3
    {
        // スクリプトに渡すグローバル関数
        public class GlobalParams
        {
            // 戻り値なし
            public void Arithmetic(int x, int y)
            {
                Console.WriteLine(x + " plus " + y + " is " + (x + y) + ".");
                Console.WriteLine(x + " minus " + y + " is " + (x - y) + ".");
                Console.WriteLine(x + " multiplied by " + y + " is " + (x * y) + ".");
                Console.WriteLine(x + " devided by " + y + " is " + (x / y) + ".");
            }

            // 戻り値あり
            public double Length(double x, double y)
            {
                return Math.Sqrt(x * x + y * y);
            }
        }

        public static void Run()
        {
            // スクリプトの実行時オプション
            var option = ScriptOptions.Default.WithImports("System");

            try
            {
                // ファイルからスクリプトを作成
                var stream = File.OpenRead("Scripts/Script3.csx");
                var script = CSharpScript.Create(stream, option, typeof(GlobalParams));


                // スクリプト実行
                var state = script.RunAsync(new GlobalParams());
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
