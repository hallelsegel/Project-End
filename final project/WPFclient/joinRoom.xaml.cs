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
    /// Interaction logic for joinRoom.xaml
    /// </summary>
    public partial class joinRoom : Window
    {
        ClientBody cl; //shared class 
        public joinRoom()
        {
            cl = (ClientBody)WPFclient.App.Current.Properties["client"];
            InitializeComponent();
            WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;
        }
        private void click_joinRoom(object sender, RoutedEventArgs e)
        {
            joinRoom window1 = new joinRoom();
            window1.Show();
            this.Close();
        }
        private void click_game(object sender, RoutedEventArgs e)
        {
            game window1 = new game();
            window1.Show();
            this.Close();
        }
        private void click_mainMenu(object sender, RoutedEventArgs e)
        {
            mainMenu window1 = new mainMenu();
            window1.Show();
            this.Close();
        }

        connectedUser secondForm;

        private void click_connectedUsers(object sender, MouseButtonEventArgs e)
        {
            if (secondForm != null)
            {
                secondForm.Close();
                secondForm = new connectedUser();
                //How ever you are passing information to the secondWindow
                secondForm.Show();
            }
            else
            {
                if (secondForm == null)
                {
                    secondForm = new connectedUser();
                    secondForm.Show();
                }
                else
                    secondForm.Activate();
            }
            
        }
    }
}
