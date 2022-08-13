using System;

namespace paragraph
{
    class Program
    {
        public int Letters(string s)
        {
            int count=s.Length ;
            foreach(int i in s)
            {
               
            if(i == ' ' || i == '.' || i == '?' || i == '!')
                {
                    count--;
                }
               
            }
            return count;

        }
        public int Words(string s)
        {
            
            int word = 0;
            for (int count = 0; count < (s.Length); count++)
            {
                if (s[count] == ' ' || s[count] == '\t' || s[count] == ',' || s[count] == '.' || s[count] == '?' || s[count] == ':' || s[count] == ';' || s[count] == '!')
                {
                    word++;
                }
                
            }
            return word;

        }
        public int Sentences(string s)
        {
            
            int sen = 0;
            for (int count = 0 ; count < (s.Length); count++)
            {
                if (s[count] == '.' || s[count] == '?' || s[count] == '!')
                {
                    sen++;
                }
                
            }
            return sen;
        }
    
        static void Main(string[] args)
        {
            string str;
            Console.WriteLine("Enter a paragraph : ");
            str = Console.ReadLine();
            Program p = new Program();
            Console.WriteLine($"Sentences : {p.Sentences(str)}");
            Console.WriteLine($"Words : {p.Words(str)}");
            Console.WriteLine($"Letters : {p.Letters(str)}");

            
        }
    }
}
