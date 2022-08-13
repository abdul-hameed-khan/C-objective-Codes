using System;
using System.ComponentModel;

namespace quiz
{
    class Program
    {
        
        public Program(int n)
        {
            for(int i = 2; i<n/2; i++)
            {
                if(n% i == 0)
                {
                    Console.WriteLine($"{n} is not prime");
                    return;
                }

            }
            Console.WriteLine($"{n} is a prinme");

        }
        
        static void Main()
        {
            new Program(17);

       

        }
    }
}
