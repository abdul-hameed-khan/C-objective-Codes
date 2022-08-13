using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Calculator
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        string previousValue = "";
        string opertor="";
        public MainWindow()
        {
            InitializeComponent();
        }

    

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Button b = sender as Button;
            txtScreen1.Text += (string)b.Content;
            if (txtScreen1.Text.Length>1 && txtScreen1.Text[1] != '.')
            {
                txtScreen1.Text = txtScreen1.Text.TrimStart('0');
            }
        }

        private void pointClick(object sender, RoutedEventArgs e)
        {
            if (!txtScreen1.Text.Contains("."))
                txtScreen1.Text += ".";
        }

        private void ClearClick(object sender, RoutedEventArgs e)
        {
            //txtScreen1.Text = txtScreen1.Text.Substring(0,txtScreen1.Text.Length-1);
            txtScreen1.Text = "0";
        }

        private void OperatorClick(object sender, RoutedEventArgs e)
        {
            previousValue = txtScreen1.Text;
            txtScreen1.Text = "0";
            opertor = (string)(sender as Button).Content;

        }

        private void EqualClick(object sender, RoutedEventArgs e)
        {
            string currentValue = txtScreen1.Text;
            if (opertor == "+")
            {
                txtScreen1.Text = (double.Parse(previousValue) + double.Parse(currentValue)).ToString();

            }
            else if (opertor == "-")
            {
                txtScreen1.Text = (double.Parse(previousValue) - double.Parse(currentValue)).ToString();

            }
            else if (opertor == "*")
            {
                txtScreen1.Text = (double.Parse(previousValue) * double.Parse(currentValue)).ToString();

            }
           else 
            {
                txtScreen1.Text = (double.Parse(previousValue) / double.Parse(currentValue)).ToString();

            }
        }
    }
}
