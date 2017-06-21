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

namespace WPFclient
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;
        }

        private void click_signUp(object sender, RoutedEventArgs e)
        {
            signUp window1 = new signUp();
            window1.Show();
            this.Close();
        }
        private void click_mainMenu(object sender, RoutedEventArgs e)
        {
            mainMenu window1 = new mainMenu();
            window1.Show();
            this.Close();
        }
        public void TextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            if (tb.Text == "UserName" || tb.Text == "Password")
            {
                tb.Text = string.Empty;
            }
        }
        private void TextBox_LostFocus_username(object sender, RoutedEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            string defaultText = "UserName";
            tb.Text = tb.Text == string.Empty ? defaultText : tb.Text;
        }
        private void TextBox_LostFocus_password(object sender, RoutedEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            string defaultText = "Password";
            tb.Text = tb.Text == string.Empty ? defaultText : tb.Text;
        }
    }
}
