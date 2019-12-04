using System;
using KeraLua;

namespace KeraLuaSample
{
    class MainClass
    {
        public static void Main()
        {
            Console.OutputEncoding = new System.Text.UTF8Encoding();

            

            // 其の壱 : スクリプトを丸々実行する
            //==================================================

            using (var state1 = new Lua())
            {
                Console.WriteLine("[ 其の壱 : スクリプトを丸々実行する ]\n");

                // OpenLibs メソッドの実行を忘れずに
                state1.OpenLibs();
                state1.LoadFile("script1.lua");
                state1.PCall(0, 0, 0);
                state1.Close();

                Console.WriteLine("\n\n");
            }

            //==================================================



            // 其の弐 : Luaステートのスタックを利用する
            //==================================================

            using (var state2 = new Lua())
            {
                Console.WriteLine("[ 其の弐 : Luaステートのスタックを利用する ]\n");

                state2.PushNil();
                state2.PushNumber(2.71);
                state2.PushNumber(3.14);
                state2.PushString("Lua");
                state2.PushBoolean(true);
                PrintStack(state2);
                state2.Close();

                Console.WriteLine("\n\n");
            }

            //==================================================



            // 其の参 : 変数の受け渡し
            //==================================================

            // Luaで定義された変数をC#で取得する
            using (var state3_1 = new Lua())
            {
                Console.WriteLine("[ 其の参(壱) : Luaで定義された変数をC#で取得する ]\n");

                state3_1.OpenLibs();
                state3_1.LoadFile("script3-1.lua");
                state3_1.PCall(0, 0, 0);

                state3_1.GetGlobal("string");
                Console.WriteLine("string = " + state3_1.ToString(-1));
                state3_1.GetGlobal("number");
                Console.WriteLine("number = " + state3_1.ToNumber(-1));
                state3_1.GetGlobal("boolean");
                Console.WriteLine("boolean = " + state3_1.ToBoolean(-1));

                state3_1.Close();

                Console.Write("\n\n\n");
            }

            // C#で設定された変数をLuaで使用する
            using (var state3_2 = new Lua())
            {
                Console.WriteLine("[ 其の参(弐) : C#で設定された変数をLuaで使用する ]\n");

                state3_2.OpenLibs();
                state3_2.LoadFile("script3-2.lua");

                state3_2.PushNumber(56);
                state3_2.SetGlobal("x");
                state3_2.PushNumber(7);
                state3_2.SetGlobal("y");

                state3_2.PCall(0, 0, 0);

                state3_2.Close();

                Console.Write("\n\n\n");
            }

            //==================================================



            // 其の肆 : 関数の受け渡し
            //==================================================

            // Luaで定義された関数をC#で使用する
            using (var state4_1 = new Lua())
            {
                Console.WriteLine("[ 其の肆(壱) : Luaで定義された関数をC#で使用する ]\n");

                state4_1.OpenLibs();
                state4_1.LoadFile("script4-1.lua");
                state4_1.PCall(0, 0, 0);

                state4_1.GetGlobal("arithmetic");
                state4_1.PushNumber(64);
                state4_1.PushNumber(36);

                state4_1.PCall(2, 4, 0);

                Console.WriteLine("sum = " + state4_1.ToNumber(-4));
                Console.WriteLine("dif = " + state4_1.ToNumber(-3));
                Console.WriteLine("mul = " + state4_1.ToNumber(-2));
                Console.WriteLine("div = " + state4_1.ToNumber(-1));

                state4_1.Close();

                Console.Write("\n\n\n");
            }

            // C#で定義された関数をLuaで使用する
            using (var state4_2 = new Lua())
            {
                Console.WriteLine("[ 其の肆(弐) : C#で定義された関数をLuaで使用する ]\n");

                state4_2.OpenLibs();
                state4_2.LoadFile("script4-2.lua");

                state4_2.PushCFunction((IntPtr int_ptr) =>
                {
                    Lua state = Lua.FromIntPtr(int_ptr);
                    double x = state.ToNumber(-2);
                    double y = state.ToNumber(-1);
                    state.Pop(-1);
                    state.PushNumber(x + y);
                    state.PushNumber(x - y);
                    state.PushNumber(x * y);
                    state.PushNumber(x / y);
                    return 4;
                });

                state4_2.SetGlobal("arithmetic");
                state4_2.PCall(0, 0, 0);

                state4_2.Close();

                Console.Write("\n\n\n");
            }

            //==================================================



            // 其の伍 : テーブルの受け渡し
            //==================================================

            // Luaで定義されたテーブルをC#で取得する
            using (var state5_1 = new Lua())
            {
                Console.WriteLine("[ 其の伍(壱) : Luaで定義されたテーブルをC#で取得する ]\n");

                state5_1.OpenLibs();
                state5_1.LoadFile("script5-1.lua");
                state5_1.PCall(0, 0, 0);

                state5_1.GetGlobal("table");
                int table_pos = state5_1.GetTop();
                state5_1.GetField(table_pos, "string");
                Console.WriteLine("string = " + state5_1.ToString(-1));
                state5_1.GetField(table_pos, "number");
                Console.WriteLine("number = " + state5_1.ToNumber(-1));
                state5_1.GetField(table_pos, "boolean");
                Console.WriteLine("boolean = " + state5_1.ToBoolean(-1));

                state5_1.Close();

                Console.Write("\n\n\n");
            }

            // C#で設定されたテーブルをLuaで使用する
            using (var state5_2 = new Lua())
            {
                Console.WriteLine("[ 其の伍(弐) : C#で設定されたテーブルをLuaで使用する ]\n");

                state5_2.OpenLibs();
                state5_2.LoadFile("script5-2.lua");

                state5_2.NewTable();
                state5_2.PushString("This is a lua script.");
                state5_2.SetField(-2, "string");
                state5_2.PushNumber(3.14159265);
                state5_2.SetField(-2, "number");
                state5_2.PushBoolean(true);
                state5_2.SetField(-2, "boolean");
                state5_2.SetGlobal("table");

                state5_2.PCall(0, 0, 0);

                state5_2.Close();

                Console.Write("\n\n\n");
            }

            //==================================================



            // 其の陸 : コルーチンを使う
            //==================================================

            using (var state6 = new Lua())
            {
                Console.WriteLine("[ 其の陸 : コルーチンを使う ]\n");

                state6.OpenLibs();
                state6.LoadFile("script6.lua");
                state6.PCall(0, 0, 0);

                Lua coroutine = state6.NewThread();
                coroutine.GetGlobal("co");

                while (coroutine.Resume(null, 0) == LuaStatus.Yield)
                    Console.WriteLine(coroutine.ToString(-1));

                state6.Close();

                Console.Write("\n\n\n");
            }

            //==================================================



            // Luaを使ってオブジェクトの内部状態を更新する
            //==================================================

            Console.WriteLine("[ Luaを使ってオブジェクトの内部状態を更新する ]\n");

            ScriptableObject Object = new ScriptableObject();
            for (int i = 0; i < 30; ++i) Object.Update();
            Object.PrintCoordinate();
            for (int i = 0; i < 20; ++i) Object.Update();
            Object.PrintCoordinate();
            for (int i = 0; i < 35; ++i) Object.Update();
            Object.PrintCoordinate();

            Console.Write("\n\n\n");

            //==================================================
        }



        // 其の弐 : スタックの内容を表示する関数
        static void PrintStack(Lua state)
        {
            Console.Write("==================================================\n");

            // 最後に格納した値のインデックスを取得・表示
            // このインデックスがスタックに積まれた値の数である.
            int stack_amount = state.GetTop();
            Console.Write("{0} values are in this stack.", stack_amount);

            Console.Write("\n--------------------------------------------------\n");

            for (int i = stack_amount; i > 0; --i)
            {
                var type = state.Type(i);
                Console.WriteLine(type switch
                {
                    LuaType.Nil =>           "NIL           : ",
                    LuaType.Boolean =>       "BOOLEAN       : " + state.ToBoolean(i),
                    LuaType.LightUserData => "LIGHTUSERDATA : ",
                    LuaType.Number =>        "NUMBER        : " + state.ToNumber(i),
                    LuaType.String =>        "STRING        : " + state.ToString(i),
                    LuaType.Table =>         "TABLE         : ",
                    LuaType.Function =>      "FUNCTION      : ",
                    LuaType.UserData =>      "USERDATA      : ",
                    LuaType.Thread =>        "THREAD        : ",
                    _ =>                     "* INVALID VALUE *"
                });
            }

            Console.Write("==================================================\n");
        }
    }
}
