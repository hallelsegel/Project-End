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
using System.Net.Sockets;
using System.Net;

namespace WPFclient
{
    /// <summary>
    /// Interaction logic for mainMenu.xaml
    /// </summary>
    public partial class mainMenu : Window
    {
        ClientBody cl; //shared class
        public mainMenu()
        {
            cl = (ClientBody)WPFclient.App.Current.Properties["client"];
            InitializeComponent();
            this.Closed += new EventHandler(theWindow_Closed);
            WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;
        }

        private void button_logout(object sender, RoutedEventArgs e)
        {
            MainWindow m = new MainWindow();
            m.Show();
            
            this.Close();
        }

        private void button_highScores(object sender, RoutedEventArgs e)
        {
            ClientBody c = new ClientBody();
            TextBox output = new TextBox();
            output.HorizontalAlignment=HorizontalAlignment.Stretch;
            output.Height=46;
            output.TextWrapping=TextWrapping.Wrap;
            output.VerticalAlignment=VerticalAlignment.Top;
            output.Width=300;
            //output.Text = c.getBestScores();
        }

        private void theWindow_Closed(object sender, System.EventArgs e)
        {
            byte[] buffer = new ASCIIEncoding().GetBytes("299");//when the window is closed, send the exit code
            cl._clientStream.Write(buffer, 0, buffer.Length);
            cl._clientStream.Flush();
        }
    }

    /*class ClientBody
    {
        /*}
        public string getBestScores()
        {
            string line="";
            byte[] buffer = new ASCIIEncoding().GetBytes("223"), rcv = new byte[3];
            _clientStream.Write(buffer, 0, buffer.Length);
            _clientStream.Flush();
            _clientStream.Read(rcv, 0, 3);
            if (!rcv.Equals("124"))
            {
                line = "Error in getting highscores. please try again";
            }
            else
            {
                line = "Highest scores: \n";
                for (int i = 0; i < 3; i++)
                {
                    rcv = new byte[2];
                    _clientSocket.Receive(rcv, rcv.Length, 0);
                    int usernameLen = 0;
                    BitConverter.ToInt32(rcv, usernameLen);
                    if (usernameLen == 0) break;
                    _clientSocket.Receive(rcv, usernameLen, 0);
                    line = line + "User " + rcv;
                    _clientSocket.Receive(rcv, rcv.Length, 0);
                    line = line + " has answered correctly " + rcv + "% of the time";
                }
            }
            Console.ReadKey();
            return line;
        }
    */
}
