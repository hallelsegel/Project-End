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
using System.Windows.Shapes;

namespace WPFclient
{
    /// <summary>
    /// Interaction logic for createRoom.xaml
    /// </summary>
    public partial class createRoom : Window
    {
        public createRoom()
        {
            InitializeComponent();
            WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;
        }
        private void click_mainMenu(object sender, RoutedEventArgs e)
        {
            mainMenu window1 = new mainMenu();
            window1.Show();
            this.Close();
        }
        private void click_inRoom(object sender, RoutedEventArgs e)
        {
            inRoom window1 = new inRoom();
            window1.Show();
            this.Close();
        }
        public void TextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            if (tb.Text == "Room Name" || tb.Text == "Num Of Players" || tb.Text == "Num Of Questions" || tb.Text == "Time For Question")
            {
                tb.Text = string.Empty;
            }
        }
        private void TextBox_LostFocus_roomName(object sender, RoutedEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            string defaultText = "Room Name";
            tb.Text = tb.Text == string.Empty ? defaultText : tb.Text;
        }
        private void TextBox_LostFocus_numPlayers(object sender, RoutedEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            string defaultText = "Num Of Players";
            tb.Text = tb.Text == string.Empty ? defaultText : tb.Text;
        }
        private void TextBox_LostFocus_numQuestions(object sender, RoutedEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            string defaultText = "Num Of Questions";
            tb.Text = tb.Text == string.Empty ? defaultText : tb.Text;
        }
        private void TextBox_LostFocus_timeQuestion(object sender, RoutedEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            string defaultText = "Time For Question";
            tb.Text = tb.Text == string.Empty ? defaultText : tb.Text;
        }
    }
}
