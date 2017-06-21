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
        public personalStats()
        {
            InitializeComponent();
            WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;
        }
        private void click_mainMenu(object sender, RoutedEventArgs e)
        {
            mainMenu window1 = new mainMenu();
            window1.Show();
            this.Close();
        }
    }
}
