using static System.Console;

namespace Roslyn
{
    class Program
    {
        public static void Main()
        {
            WriteLine();



            // 其の壱 : スクリプトを丸々実行する
            WriteLine("[ Program1-1.cs ]");
            Program1_1.Run();
            WriteLine();

            WriteLine("[ Program1-2.cs ]");
            Program1_2.Run();
            WriteLine();



            // 其の弐 : 変数の受け渡し
            WriteLine("[ Program2-1.cs ]");
            Program2_1.Run();
            WriteLine();

            WriteLine("[ Program2-2.cs ]");
            Program2_2.Run();
            WriteLine();



            // 其の参 : スクリプトに関数を渡す
            WriteLine("[ Program3.cs ]");
            Program3.Run();
            WriteLine();



            // 其の肆 : スクリプトのオブジェクト指向的(?)な使い方
            WriteLine("[ Program4.cs ]");
            Program4.Run();
            WriteLine();
        }
    }
}
