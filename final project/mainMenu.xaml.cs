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
    /// Interaction logic for mainMenu.xaml
    /// </summary>
    public partial class mainMenu : Window
    {
        public mainMenu()
        {
            InitializeComponent();
            WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;
        }

        private void click_createRoom(object sender, RoutedEventArgs e)
        {
            createRoom window1 = new createRoom();
            window1.Show();
            this.Close();
        }

        private void click_joinRoom(object sender, RoutedEventArgs e)
        {
            joinRoom window1 = new joinRoom();
            window1.Show();
            this.Close();
        }

        private void click_highScores(object sender, RoutedEventArgs e)
        {
            highScores window1 = new highScores();
            window1.Show();
            this.Close();
        }

        private void click_personalStats(object sender, RoutedEventArgs e)
        {
            personalStats window1 = new personalStats();
            window1.Show();
            this.Close();
        }

        private void click_logOut(object sender, RoutedEventArgs e)
        {
            MainWindow window1 = new MainWindow();
            window1.Show();
            this.Close();
        }
    }
}
