//////////////////////////////////////////////////
//
//    Program1-2.cs
//    C# スクリプトを実行し,戻り値を取得する
//

using System;
using System.IO;

using Microsoft.CodeAnalysis.Scripting;
using Microsoft.CodeAnalysis.CSharp.Scripting;

namespace Roslyn
{
    class Program1_2
    {
        public static void Run()
        {
            // スクリプトの実行時オプション
            var option = ScriptOptions.Default.WithImports("System");

            try
            {
                // ファイルからスクリプトを作成
                var stream = File.OpenRead("Scripts/Script1-2.csx");
                var script = CSharpScript.Create(stream, option);

                // スクリプト実行
                var result = script.RunAsync().Result;

                // 取得した戻り値を出力
                Console.WriteLine(result.ReturnValue);
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
