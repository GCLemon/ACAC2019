//////////////////////////////////////////////////
//
//    Program1-1.cs
//    C# スクリプトを丸々実行する
//

using System;
using System.IO;

using Microsoft.CodeAnalysis.Scripting;
using Microsoft.CodeAnalysis.CSharp.Scripting;

namespace Roslyn
{
    class Program1_1
    {
        public static void Run()
        {
            // スクリプトの実行時オプション
            var option = ScriptOptions.Default.WithImports("System");

            try
            {
                // ファイルからスクリプトを作成
                var stream = File.OpenRead("Scripts/Script1-1.csx");
                var script = CSharpScript.Create(stream, option);

                // スクリプト実行
                script.RunAsync();
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
