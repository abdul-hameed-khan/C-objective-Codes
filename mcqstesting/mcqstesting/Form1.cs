using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace mcqstesting
{
    public partial class Form1 : Form
    {
        TextReader reader;
        public Form1()
        {
            InitializeComponent();
            reader = new StreamReader("C:/Users/Zulfiqar/Desktop/file1.txt");
        }

        private void buttonNext_click(object sender, EventArgs e)
        {
            StreamReader sw = new StreamReader("C:\\Users\\Zulfiqar\\Desktop\\file1.txt");

            //Use write method to write the text
            string text = sw.ReadToEnd();
            richTextBox1.Text = text;
            //always close your stream
            sw.Close();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            
        }

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {
            //  richTextBox1.Text = File.ReadAllText("C:\\Users\\Zulfiqar\\Desktop\\New Text Document (2).txt");
            //Use StreamWriter class.
           /* StreamReader sw = new StreamReader("C:\\Users\\Zulfiqar\\Desktop\\file1.txt");

            //Use write method to write the text
            string text=sw.ReadLine();
            richTextBox1.Text = text;
            //always close your stream
            sw.Close();*/
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {

        }
    }
}
