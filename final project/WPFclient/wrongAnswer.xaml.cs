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
    /// Interaction logic for wrongAnswer.xaml
    /// </summary>
    public partial class wrongAnswer : Window
    {
        public wrongAnswer()
        {
            InitializeComponent();
            WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;
            waitAndContinue();
        }
        public async Task waitAndContinue()
        {
            await Task.Delay(500);
            game window1 = new game();
            window1.Show();
            this.Close();
        }
    }
}
