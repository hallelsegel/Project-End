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
    /// Interaction logic for highScores.xaml
    /// </summary>
    public partial class highScores : Window
    { 
        ClientBody cl; //shared class 
        public highScores()
        {
            cl = (ClientBody)WPFclient.App.Current.Properties["client"];
            this.Closed += new EventHandler(theWindow_Closed);
            InitializeComponent();
            UserName.Content = cl._username;
            getBestScores();
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
        private void theWindow_Closed(object sender, System.EventArgs e)
        {
            byte[] buffer = new ASCIIEncoding().GetBytes("299");//when the window is closed, send the exit code
            cl._clientStream.Write(buffer, 0, buffer.Length);
            cl._clientStream.Flush();
        }

        public void getBestScores()
        {
            string[] lines = new string[3];
            byte[] buffer = new ASCIIEncoding().GetBytes("223"), rcv = new byte[3];
            cl._clientStream.Write(buffer, 0, buffer.Length); //send message code to server
            cl._clientStream.Flush();
            cl._clientStream.Read(rcv, 0, 3);
            string answer = System.Text.Encoding.UTF8.GetString(rcv, 0, 3);
            if (answer != "124") //124 == correct answer code to the best scores request from the server
            {
                System.Windows.MessageBox.Show(this, "Error in getting highscores. please try again");
            }
            else
            {
                for (int i = 0; i < 3; i++)
                {
                    rcv = new byte[2];
                    cl._clientStream.Read(rcv, 0, 2);
                    int usernameLen = ((int)rcv[0] - 48) * 10 + (int)rcv[1] - 48;
                    if (usernameLen == 0) break;
                    rcv = new byte[usernameLen];
                    cl._clientStream.Read(rcv, 0, usernameLen);
                    answer = System.Text.Encoding.UTF8.GetString(rcv);
                    lines[i] = "User " + answer;
                    rcv = new byte[6];
                    cl._clientStream.Read(rcv, 0, 3);
                    answer = System.Text.Encoding.UTF8.GetString(rcv);
                    lines[i] = lines[i] + " has answered\n correctly " + Int32.Parse(answer) + "% of the time";
                }
                first.Content = lines[0]; second.Content = lines[1]; third.Content = lines[2];
            }
        }
    }

    public class User
    {
        public string Name { get; set; }

        public int Score { get; set; }
    }
}
