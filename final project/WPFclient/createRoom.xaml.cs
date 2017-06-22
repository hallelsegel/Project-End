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
        ClientBody cl; //shared class 
        public createRoom()
        {
            cl = (ClientBody)WPFclient.App.Current.Properties["client"];
            InitializeComponent();
            UserName.Content = cl._username;
            this.Closed += new EventHandler(theWindow_Closed);
            WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;
        }
        private void click_mainMenu(object sender, RoutedEventArgs e)
        {//move back to main menu
            int i;
            for (i = 0; i < WPFclient.App.Current.Windows.Count; i++) if (WPFclient.App.Current.Windows[i].ToString() == "WPFclient.mainMenu") break;
            if (i == WPFclient.App.Current.Windows.Count) //if there is mainMenu open already
            {
                mainMenu m = new mainMenu(); //else create one and open it
                m.Show();
            }
            else WPFclient.App.Current.Windows[i].Show();
            this.Hide();
        }
        private void click_inRoom(object sender, RoutedEventArgs e)
        {
            if (sendCreateRoom())
            {
                int i;
                for (i = 0; i < WPFclient.App.Current.Windows.Count; i++) if (WPFclient.App.Current.Windows[i].ToString() == "WPFclient.inRoom") break;
                if (i == WPFclient.App.Current.Windows.Count) //if there is mainMenu open already
                {
                    inRoom m = new inRoom(); //else create one and open it
                    m.Show();
                }
                else WPFclient.App.Current.Windows[i].Show();
                this.Hide();
            }
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
        private void theWindow_Closed(object sender, System.EventArgs e)
        {
            byte[] buffer = new ASCIIEncoding().GetBytes("299");//when the window is closed, send the exit code
            cl._clientStream.Write(buffer, 0, buffer.Length);
            cl._clientStream.Flush();
        }
        private bool sendCreateRoom()
        {
            int p = 0, q=0, t =0;
            if (name.Text == "Room Name" || players.Text == "Num Of Players" || questions.Text == "Num Of Questions" || time.Text == "Time For Question")
                System.Windows.MessageBox.Show(this, "You must fill all of the fields!");
            else if (!Int32.TryParse(players.Text, out p) ||
                    !Int32.TryParse(questions.Text, out q) ||
                    !Int32.TryParse(time.Text, out t))
                System.Windows.MessageBox.Show(this, "Numeric fields do not have numeric values!");
            else if (name.Text.Length < 1) System.Windows.MessageBox.Show(this, "You must have a name for your room!");
            else if (name.Text.Length > 99) System.Windows.MessageBox.Show(this, "Your room's name must be up to 99 characters!");
            else if (p < 1) System.Windows.MessageBox.Show(this, "You can't have 0 players playing!");
            else if (p > 9) System.Windows.MessageBox.Show(this, "You can only have up to 9 players!");
            else if (q < 1) System.Windows.MessageBox.Show(this, "You can't have 0 questions!");
            else if (q > 10) System.Windows.MessageBox.Show(this, "You can only have up to 10 questions!"); //our current number of available questions...
            else if (t < 1) System.Windows.MessageBox.Show(this, "You can't have 0 seconds to answer!");
            else if (t > 9) System.Windows.MessageBox.Show(this, "You can only have up to 99 seconds!");
            else
            {
                string nameLen = (name.Text.Length).ToString().PadLeft(2, '0');
                byte[] buffer = new ASCIIEncoding().GetBytes("213" + nameLen + name.Text + players.Text + questions.Text.PadLeft(2, '0')+ time.Text.PadLeft(2, '0'));
                byte[] rcv = new byte[4];
                cl._clientStream.Write(buffer, 0, buffer.Length);//send message code to server
                cl._clientStream.Flush();
                cl._clientStream.Read(rcv, 0, 4);
                string answer = System.Text.Encoding.UTF8.GetString(rcv, 0, 4);
                if (answer != "1140") //1140 == personal stats correct answer code from server (success)
                {
                    System.Windows.MessageBox.Show(this, "Error in getting personal stats. please try again");
                    return false; //failure
                }
                return true; //success
            }
            return false; //one of the input checkers went off
        }
    }
}
