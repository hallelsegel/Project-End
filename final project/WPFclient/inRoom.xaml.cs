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
    /// Interaction logic for inRoom.xaml
    /// </summary>
    public partial class inRoom : Window
    {
        ClientBody cl; //shared class 
        public inRoom()
        {
            cl = (ClientBody)WPFclient.App.Current.Properties["client"];
            InitializeComponent();
            UserName.Content = cl._username;
            WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;
        }
        private void click_game(object sender, RoutedEventArgs e)
        {//open a game
            int i;
            for (i = 0; i < WPFclient.App.Current.Windows.Count; i++) if (WPFclient.App.Current.Windows[i].ToString() == "WPFclient.game") break;
            if (i == WPFclient.App.Current.Windows.Count) //if there is mainMenu open already
            {
                game g = new game(); //else create one and open it
                g.Show();
            }
            else WPFclient.App.Current.Windows[i].Show();
            this.Hide();
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
        private void adminLeft(object sender, RoutedEventArgs e)
        {  // this function needs to check if the creator disconnected
            //if (true) // this needs to check if the creator disconnected
            //{
            //    this.Background = new ImageBrush(new BitmapImage(new Uri(@"pack://application:,,,/WPFclient;component/Resources/BG_red.png")));
            //    textExit.Visibility = Visibility.Visible;
            //    joinerExit.Visibility = Visibility.Visible;
            //    creatorStrartButton.Visibility = Visibility.Hidden;
            //    creatorExitButton.Visibility = Visibility.Hidden;
            //}
        }
    }
}
