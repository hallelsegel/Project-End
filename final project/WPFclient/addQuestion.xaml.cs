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
    /// Interaction logic for addQuestion.xaml
    /// </summary>
    public partial class addQuestion : Window
    {
        ClientBody cl;
        public addQuestion()
        {
            cl = cl = (ClientBody)WPFclient.App.Current.Properties["client"];
            InitializeComponent();
            WindowStartupLocation = WindowStartupLocation.CenterScreen;
        }
        public void click_mainMenu(object sender, RoutedEventArgs e)
        {
            mainMenu m = new mainMenu();
            m.Show();
            this.Close();
        }
        public void click_submitQuestion(object sender, RoutedEventArgs e)
        {
            string qTextLen = (Question.Text.Length).ToString().PadLeft(2, '0');
            string ans1Len = (Answer1.Text.Length).ToString().PadLeft(2, '0');
            string ans2Len = (Answer2.Text.Length).ToString().PadLeft(2, '0');
            string ans3Len = (Answer3.Text.Length).ToString().PadLeft(2, '0');
            string ans4Len = (Answer4.Text.Length).ToString().PadLeft(2, '0');

            byte[] buffer = new ASCIIEncoding().GetBytes("230" + qTextLen + Question.Text + ans1Len + Answer1.Text + ans2Len + Answer2.Text + ans3Len + Answer3.Text + ans4Len + Answer4.Text), rcv = new byte[4]; 
            cl._clientStream.Write(buffer, 0, buffer.Length);
            cl._clientStream.Flush();
            cl._clientStream.Read(rcv, 0, 4);
            string answer = System.Text.Encoding.UTF8.GetString(rcv, 0, 3);
            if (answer != "130") MessageBox.Show(this, "Error in sending the question");//130 == answer code for add question
            else
            {
                answer = System.Text.Encoding.UTF8.GetString(rcv, 3 ,1);
                if (answer == "0") MessageBox.Show(this, "Question added succesfully! Thanks for contributing!");//1300 == success
                else MessageBox.Show(this, "Error in adding the question");//1301 == failure
            }
            addQuestion q = new addQuestion();//reset window
            q.Show();
            this.Close(); 
        }
        private void TextBox_LostFocus_Question(object sender, RoutedEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            string defaultText = "Question";
            tb.Text = tb.Text == string.Empty ? defaultText : tb.Text;
        }
        private void TextBox_LostFocus_Answer1(object sender, RoutedEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            string defaultText = "Answer number 1";
            tb.Text = tb.Text == string.Empty ? defaultText : tb.Text;
        }
        private void TextBox_LostFocus_Answer2(object sender, RoutedEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            string defaultText = "Answer number 2";
            tb.Text = tb.Text == string.Empty ? defaultText : tb.Text;
        }
        private void TextBox_LostFocus_Answer3(object sender, RoutedEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            string defaultText = "Answer number 3";
            tb.Text = tb.Text == string.Empty ? defaultText : tb.Text;
        }
        private void TextBox_LostFocus_Answer4(object sender, RoutedEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            string defaultText = "Answer number 4";
            tb.Text = tb.Text == string.Empty ? defaultText : tb.Text;
        }
        public void TextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            if (tb.Text == "UserName" || tb.Text == "Password" || tb.Text == "Email")
            {
                tb.Text = string.Empty;
            }
        }
    }
}
