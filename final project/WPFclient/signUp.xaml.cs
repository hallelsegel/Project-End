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
    /// Interaction logic for signUp.xaml
    /// </summary>
    public partial class signUp : Window
    {
        ClientBody cl; //shared class 
        public signUp()
        {
            cl = (ClientBody)WPFclient.App.Current.Properties["client"];
            InitializeComponent();
            WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;
        }
        private void button_send(object sender, RoutedEventArgs e)
        {
            try
            {
                string usernameLen = (username.Text.Length).ToString().PadLeft(2, '0');
                string passLen = (pass.Text.Length).ToString().PadLeft(2, '0');
                string emailLen = (email.Text.Length).ToString().PadLeft(2, '0');
                byte[] buffer = new ASCIIEncoding().GetBytes("203" + usernameLen + username.Text + passLen + pass.Text + emailLen + email.Text);
                byte[] rcv = new byte[4];
                cl._clientStream.Write(buffer, 0, buffer.Length);
                cl._clientStream.Flush();
                cl._clientStream.Read(rcv, 0, 4);
                string answer = System.Text.Encoding.UTF8.GetString(rcv);
                if (answer == "1040") /*success*/ System.Windows.MessageBox.Show(this, "Sign up succesful, you are can now sign in, " + username.Text + "!");
                else if (answer == "1041") /*illegal password*/ 
                    System.Windows.MessageBox.Show(this, "Sign up unsuccesful, the password has to be longer than 4 characters, include at least: 1 lowercase letter, 1 uppercase letter, 1 number and no spaces");
                else if (answer == "1042") /*username taken*/
                    System.Windows.MessageBox.Show(this, "Sign up unsuccesful, username is already taken");
                else if (answer == "1043") /*illegal username*/ 
                    System.Windows.MessageBox.Show(this, "Sign up unsuccesful, the username has to be begin with a letter, and include no spaces. ");
                else if (answer == "1044") /*other*/
                    System.Windows.MessageBox.Show(this, "Sign in unsuccesful, for no known reason");
                MainWindow m = new MainWindow();
                m.Show();
                this.Close();
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(this, ex.Message);
            }
        }
        public void TextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            if (tb.Text == "UserName" || tb.Text == "Password" || tb.Text == "Email")
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

        private void TextBox_LostFocus_Email(object sender, RoutedEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            string defaultText = "Email";
            tb.Text = tb.Text == string.Empty ? defaultText : tb.Text;
        }
    }
}
