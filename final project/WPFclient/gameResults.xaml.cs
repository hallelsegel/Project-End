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
    /// Interaction logic for gameResults.xaml
    /// </summary>
    public partial class gameResults : Window
    {
        int qNum;
        ClientBody cl; //shared class 
        public gameResults(int questionNum)
        {
            qNum = questionNum;
            cl = (ClientBody)WPFclient.App.Current.Properties["client"];
            InitializeComponent();
            getResults();
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
            this.Close();
        }
        private void getResults()
        {
            byte[] rcv = new byte[1];
            cl._clientStream.Read(rcv, 0, rcv.Length);
            int userNum = Int32.Parse(Encoding.UTF8.GetString(rcv, 0, rcv.Length));
            userScoreList.Items.Clear();
            userNameList.Items.Clear();
            for (int i=0;i<userNum;i++)
            {
                rcv = new byte[2];
                cl._clientStream.Read(rcv, 0, rcv.Length);
                int userLen = Int32.Parse(Encoding.UTF8.GetString(rcv, 0, rcv.Length));
                rcv = new byte[userLen];
                cl._clientStream.Read(rcv, 0, rcv.Length);
                ListBoxItem user = new ListBoxItem();
                user.Content = Encoding.UTF8.GetString(rcv, 0, rcv.Length);
                userNameList.Items.Add(user);
                rcv = new byte[2];
                cl._clientStream.Read(rcv, 0, rcv.Length);
                ListBoxItem score = new ListBoxItem();
                score.Content = Int32.Parse(Encoding.UTF8.GetString(rcv, 0, rcv.Length)) + " / " + qNum;
                userScoreList.Items.Add(score);
            }
        }

    }
}
