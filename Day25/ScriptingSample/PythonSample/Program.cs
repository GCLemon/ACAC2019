using System;

using IronPython.Hosting;
using IronPython.Runtime;
using Microsoft.Scripting.Hosting;

namespace PythonSample
{
    class MainClass
    {
        public static void Main()
        {
            // Python エンジンを生成
            ScriptEngine engine = Python.CreateEngine();



            // 其の壱 : スクリプトを丸々実行する
            //==================================================

            {
                Console.WriteLine("[ 其の壱 : スクリプトを丸々実行する ]\n");

                // スコープとソースを生成
                ScriptScope scope1 = engine.CreateScope();
                ScriptSource source1 = engine.CreateScriptSourceFromFile("script1.py");

                // スクリプトの実行
                source1.Execute(scope1);

                Console.WriteLine("\n\n");
            }

            //==================================================



            // 其の弐 : 変数の受け渡し
            //==================================================

            {
                Console.WriteLine("[ 其の弐(壱) : IronPythonで定義された変数をC#で取得する ]\n");

                // スコープとソースを生成
                ScriptScope scope2_1 = engine.CreateScope();
                ScriptSource source2_1 = engine.CreateScriptSourceFromFile("script2-1.py");

                // スクリプトの実行(読み込み)
                source2_1.Execute(scope2_1);

                // グローバル変数の読み出し
                Console.WriteLine("number = " + scope2_1.GetVariable("number"));
                Console.WriteLine("string = " + scope2_1.GetVariable("string"));
                Console.WriteLine("boolean = " + scope2_1.GetVariable("boolean"));

                Console.WriteLine("\n\n");
            }

            {
                Console.WriteLine("[ 其の弐(弐) : C#で設定された変数をIronPythonで使用する ]\n");

                // スコープとソースを生成
                ScriptScope scope2_2 = engine.CreateScope();
                ScriptSource source2_2 = engine.CreateScriptSourceFromFile("script2-2.py");

                // グローバル変数の設定
                scope2_2.SetVariable("number", 3.1415925);
                scope2_2.SetVariable("string", "This is a iron python script.");
                scope2_2.SetVariable("boolean", true);

                // スクリプトの実行
                source2_2.Execute(scope2_2);

                Console.WriteLine("\n\n");
            }

            //==================================================



            // 其の参 : 関数の受け渡し
            //==================================================

            {
                Console.WriteLine("[ 其の参(壱) : IronPythonで定義された関数をC#で使用する ]\n");

                // スコープとソースを生成
                ScriptScope scope3_1 = engine.CreateScope();
                ScriptSource source3_1 = engine.CreateScriptSourceFromFile("script3-1.py");

                // スクリプトの実行(読み込み)
                source3_1.Execute(scope3_1);

                // 戻り値の持たない関数の実行
                Action print_something = scope3_1.GetVariable("print_something");
                print_something();

                // 戻り値をもつ関数の実行
                Func<string> return_something = scope3_1.GetVariable("return_something");
                Console.WriteLine(return_something());

                Console.WriteLine("\n\n");
            }

            {
                Console.WriteLine("[ 其の参(弐) : C#で定義された関数をIronPythonで使用する ]\n");

                // スコープとソースを生成
                ScriptScope scope3_2 = engine.CreateScope();
                ScriptSource source3_2 = engine.CreateScriptSourceFromFile("script3-2.py");

                // 戻り値の持たない関数の設定
                scope3_2.SetVariable("print_something", new Action(PrintSomething));

                // 戻り値をもつ関数の設定
                scope3_2.SetVariable("return_something", new Func<string>(ReturnSomething));

                // スクリプトの実行
                source3_2.Execute(scope3_2);

                Console.WriteLine("\n\n");
            }

            //==================================================



            // 其の肆 : C#で作成されたインスタンスをIronPythonで使用する
            //==================================================

            {
                Console.WriteLine("[ 其の肆 : C#で作成されたインスタンスをIronPythonで使用する ]\n");

                // スコープとソースを生成
                ScriptScope scope4 = engine.CreateScope();
                ScriptSource source4 = engine.CreateScriptSourceFromFile("script4.py");

                // SomeObjectクラスのインスタンスの作成
                SomeObject obj = new SomeObject();

                // グローバル変数の設定
                scope4.SetVariable("some_object", obj);

                // スクリプトの実行
                source4.Execute(scope4);

                // インスタンスのメンバ変数の値を確認
                Console.WriteLine(obj.Message);

                Console.WriteLine("\n\n");
            }

            //==================================================



            // 其の伍 : コルーチンを使う
            //==================================================

            {
                Console.WriteLine("[ 其の伍 : コルーチンを使う ]\n");

                // スコープとソースを生成
                ScriptScope scope5 = engine.CreateScope();
                ScriptSource source5 = engine.CreateScriptSourceFromFile("script5.py");

                // スクリプトの実行
                source5.Execute(scope5);

                // コルーチンの読み出し・実行
                Func<PythonGenerator> coroutine = scope5.GetVariable("coroutine");
                PythonGenerator generator = coroutine();
                generator.next();
                generator.next();
                generator.next();

                Console.WriteLine("\n\n");
            }

            //==================================================
        }



        // 適当に作ったメソッド
        public static void PrintSomething()
        {
            Console.WriteLine("This method is defined in C#.");
        }

        // 適当に作ったメソッド其の弐
        public static string ReturnSomething()
        {
            return "Here, this is the return value from C#.";
        }
    }


    // 適当に作ったクラス
    public class SomeObject
    {
        public string Message;

        public void PrintSomething()
        {
            Console.WriteLine("Method \"PrintSomething\" called.");
        }
    }
}
