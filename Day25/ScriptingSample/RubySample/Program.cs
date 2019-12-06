using System;

using IronRuby;
using Microsoft.Scripting.Hosting;

namespace RubySample
{
    class MainClass
    {
        public static void Main()
        {
            // Ruby エンジンを生成
            ScriptEngine engine = Ruby.CreateEngine();

            // スコープとソースを生成
            ScriptScope scope = engine.CreateScope();
            ScriptSource source = engine.CreateScriptSourceFromFile("script.rb");

            // グローバル変数を設定
            scope.SetVariable("number1", 3.14159265);
            scope.SetVariable("number2", 2.71828182);
            scope.SetVariable("string", "This is a ruby script.");
            scope.SetVariable("boolean", true);
            scope.SetVariable("some_object", new SomeObject());

            // 実行した後の戻り値を出力
            Console.WriteLine("[ Output from Ruby ]\n");
            source.Execute(scope);

            // グローバル変数の取得
            Console.Write("\n\n\n");
            Console.WriteLine("[ Output from C# ]\n");
            Console.WriteLine(scope.GetVariable("number_ret"));
            Console.WriteLine(scope.GetVariable("string_ret"));
            Console.WriteLine(scope.GetVariable("boolean_ret"));
        }
    }

    // 適当に作ったクラス
    public class SomeObject
    {
        public void PrintSomething()
        {
            Console.WriteLine("Method \"PrintSomething\" called.");
        }
    }
}
