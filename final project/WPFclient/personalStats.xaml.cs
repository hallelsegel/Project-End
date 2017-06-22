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
    /// Interaction logic for personalStats.xaml
    /// </summary>
    public partial class personalStats : Window
    {           
        ClientBody cl; //shared class 
        public personalStats()
        {
            cl = (ClientBody)WPFclient.App.Current.Properties["client"];
            InitializeComponent();
            UserName.Content = cl._username;
            this.Closed += new EventHandler(theWindow_Closed);
            getPersonalStats();
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
        private void getPersonalStats()
        {
            string[] lines = new string[3];
            byte[] buffer = new ASCIIEncoding().GetBytes("225"), rcv = new byte[3];
            cl._clientStream.Write(buffer, 0, buffer.Length);//send message code to server
            cl._clientStream.Flush();
            cl._clientStream.Read(rcv, 0, 3);
            string answer = System.Text.Encoding.UTF8.GetString(rcv, 0, 3);
            if (answer != "126") //126 == personal stats correct answer code from server
            {
                System.Windows.MessageBox.Show(this, "Error in getting personal stats. please try again");
            }
            else
            {
                rcv = new byte[4]; //number of games
                cl._clientStream.Read(rcv, 0, 4);
                answer = System.Text.Encoding.UTF8.GetString(rcv, 0, 4);
                games.Content = (Int32.Parse(answer) != 0) ? answer.TrimStart(new Char[] { '0' }) : "None. Go play some right now!";
                rcv = new byte[6]; //number of correct answers
                cl._clientStream.Read(rcv, 0, 6);
                answer = System.Text.Encoding.UTF8.GetString(rcv, 0, 6);
                correct.Content = Int32.Parse(answer);
                rcv = new byte[6]; //number of wrong answers
                cl._clientStream.Read(rcv, 0, 6);
                answer = System.Text.Encoding.UTF8.GetString(rcv, 0, 6);
                wrong.Content = Int32.Parse(answer);
                rcv = new byte[4]; //success in percentage
                cl._clientStream.Read(rcv, 0, 4);
                answer = System.Text.Encoding.UTF8.GetString(rcv, 0, 4);
                avgTime.Content = Double.Parse(answer)/100.0;

            }
        }
    }
}
