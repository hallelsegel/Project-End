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
            ClientBody cl; //shared class 
        public wrongAnswer()
        {
            cl = (ClientBody)WPFclient.App.Current.Properties["client"];
            InitializeComponent();
            this.Closed += new EventHandler(theWindow_Closed);
            WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;
            waitAndContinue();
        }
        public async Task waitAndContinue()
        {//move back to the game
            await Task.Delay(500);
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
        private void theWindow_Closed(object sender, System.EventArgs e)
        {
            byte[] buffer = new ASCIIEncoding().GetBytes("299");//when the window is closed, send the exit code
            cl._clientStream.Write(buffer, 0, buffer.Length);
            cl._clientStream.Flush();
        }
    }
}
