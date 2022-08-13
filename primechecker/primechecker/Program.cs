using System;

namespace prmechecking
{
    class Program
    {
        public static bool IsPrime(int n, int i)
        {
            if (n == 0 || n == 1)
                return false;
            if (n % i == 0)
                return false;
            if (i*i > n)
                return true;

            return IsPrime(n, i + 1);

        }

        static void Main(string[] args)
        {
            Console.Write("Enter a number: ");
            int num = Convert.ToInt32(Console.ReadLine());
            Console.WriteLine(IsPrime(num, 2));
        }
    }


}