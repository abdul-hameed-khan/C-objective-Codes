using System;

namespace ConsoleApp1
{
    class Program
    {
        static void Main()
        {
            Console.WriteLine("Hello World\f");
            Console.WriteLine("ABDUL HAMEED KHAN");
            Console.WriteLine("please enter\f your age : ");
            int age = Convert.ToInt32(Console.ReadLine());
            Console.WriteLine("your age is {0}",age);
            
            var arr = new int[5] { 1,2,3,4,5 };
            Console.WriteLine(Add(arr,2, 6));
        }

        static int Add(int[] ar,int a, int b)
        {
            var total = 0;
            for( int i = 0; i<5; i++)
            {
                total = total + ar[i];
            }
            Console.WriteLine(a + b);
            return total;
        }
    }
}
