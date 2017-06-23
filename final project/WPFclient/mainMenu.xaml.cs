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
        ClientBody cl;
        public mainMenu()
        {
            cl = (ClientBody)WPFclient.App.Current.Properties["client"];
            InitializeComponent();
            UserName.Content = cl._username;
            WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;
        }

        private void click_logOut(object sender, RoutedEventArgs e)
        {
            byte[] buffer = new ASCIIEncoding().GetBytes("201");//when the window is closed, send the exit code
            cl._clientStream.Write(buffer, 0, buffer.Length);
            cl._clientStream.Flush();
            MainWindow m = new MainWindow();
            m.Show();
            this.Close();
        }
                
        private void click_createRoom(object sender, RoutedEventArgs e)
        {
            int i;
            for (i = 0; i < WPFclient.App.Current.Windows.Count; i++) if (WPFclient.App.Current.Windows[i].ToString() == "WPFclient.createRoom") break;
            if (i == WPFclient.App.Current.Windows.Count) //if there is createRoom open already
            {
                createRoom c = new createRoom(); //else create one and open it
                c.Show();
            }
            else WPFclient.App.Current.Windows[i].Show();
            this.Close();
        }

        private void click_joinRoom(object sender, RoutedEventArgs e)
        {
            int i;
            for (i = 0; i < WPFclient.App.Current.Windows.Count; i++) if (WPFclient.App.Current.Windows[i].ToString() == "WPFclient.joinRoom") break;
            if (i == WPFclient.App.Current.Windows.Count) //if there is createRoom open already
            {
                joinRoom j = new joinRoom(); //else create one and open it
                j.Show();
            }
            else WPFclient.App.Current.Windows[i].Show();
            this.Close();
        }

        private void click_highScores(object sender, RoutedEventArgs e)
        {
            int i;
            for (i = 0; i < WPFclient.App.Current.Windows.Count; i++) if (WPFclient.App.Current.Windows[i].ToString() == "WPFclient.highScores") break;
            if (i == WPFclient.App.Current.Windows.Count) //if there is createRoom open already
            {
                highScores h = new highScores(); //else create one and open it
                h.Show();
            }
            else WPFclient.App.Current.Windows[i].Show();
            this.Close();
        }

        private void click_personalStats(object sender, RoutedEventArgs e)
        {
            int i;
            for (i = 0; i < WPFclient.App.Current.Windows.Count; i++) if (WPFclient.App.Current.Windows[i].ToString() == "WPFclient.personalStats") break;
            if (i == WPFclient.App.Current.Windows.Count) //if there is createRoom open already
            {
                personalStats p = new personalStats(); //else create one and open it
                p.Show();
            }
            else WPFclient.App.Current.Windows[i].Show();
            this.Close();
        }
    }
}
