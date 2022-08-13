using System;

namespace tableprint
{
    class Table
    {
        public static void TablePrint(int num = 2, int start = 1, int end = 10)
        {
            for (int i = start; i <= end; i++)
            {
                Console.WriteLine($"{num} X {i} = {num * i}");
            }
        }
    
    
        static void Main(string[] args)
        {  
            Console.WriteLine("if you want to enter only a number for table press 1");
            Console.WriteLine("if you want to give number, start and end of table press 2 ");
            Console.WriteLine("if you want to give start only press 3");
            Console.WriteLine("if you want to give end only press 4");
            Console.WriteLine("if you want default press 5");
            int n = Convert.ToInt32(Console.ReadLine());

            if (n == 1)
            {
                Console.WriteLine("Enter a number for table ");
                int i = Convert.ToInt32(Console.ReadLine());
                TablePrint(i);
            }
            else if (n == 2)
            {
                Console.WriteLine("Enter a number for table ");
                int i = Convert.ToInt32(Console.ReadLine());
                Console.WriteLine("Enter a start ");
                int j = Convert.ToInt32(Console.ReadLine());
                Console.WriteLine("Enter end ");
                int k = Convert.ToInt32(Console.ReadLine());
                TablePrint(i,j,k);
            }
            else if (n == 3)
            {
                Console.WriteLine("Enter a number for table ");
                int i = Convert.ToInt32(Console.ReadLine());
                Console.WriteLine("Enter a start ");
                int j = Convert.ToInt32(Console.ReadLine());
               
                TablePrint(i, j);
            }
            else if (n == 4)
            {
                Console.WriteLine("Enter a number for table ");
                int i = Convert.ToInt32(Console.ReadLine());
                Console.WriteLine("Enter end ");
                int j = Convert.ToInt32(Console.ReadLine());

                TablePrint(i, start:1,j);
            }
            else if(n==5)
            {
                TablePrint();
            }
            else
            {
                Console.WriteLine("plz enter a correct number ");
            }
  

        }
    }
}
