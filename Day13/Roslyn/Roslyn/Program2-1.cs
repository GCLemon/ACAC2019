//////////////////////////////////////////////////
//
//    Program2-1.cs
//    スクリプト側で定義されたグローバル変数を
//    プログラム側で取得する
//

using System;
using System.IO;

using Microsoft.CodeAnalysis.Scripting;
using Microsoft.CodeAnalysis.CSharp.Scripting;

namespace Roslyn
{
    public class Program2_1
    {
        public static void Run()
        {
            // スクリプトの実行時オプション
            var option = ScriptOptions.Default.WithImports("System");

            try
            {
                // ファイルからスクリプトを作成
                var stream = File.OpenRead("Scripts/Script2-1.csx");
                var script = CSharpScript.Create(stream, option);

                // スクリプト実行
                var state = script.RunAsync().Result;

                // スクリプト実行後の変数を出力
                var Number = state.GetVariable("Number");
                Console.WriteLine("Number(" + Number.Type + ") = " + Number.Value);
                var String = state.GetVariable("String");
                Console.WriteLine("String(" + String.Type + ") = " + String.Value);
                var Boolean = state.GetVariable("Boolean");
                Console.WriteLine("Boolean(" + Boolean.Type + ") = " + Boolean.Value);
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
