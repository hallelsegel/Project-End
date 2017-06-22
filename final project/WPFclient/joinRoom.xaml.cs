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
            UserName.Content = cl._username;
            getRooms(); //initial room list request
            WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;

            System.Windows.MessageBox.Show(this, "Double-click on any room to see which players are in it right now!");
        }
        private void click_joinRoom(object sender, RoutedEventArgs e)
        {
            
        }
        private void click_refresh(object sender, RoutedEventArgs e)
        {
            getRooms();
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
        private void getRooms()
        {
            roomsList.Items.Clear();
            byte[] buffer = new ASCIIEncoding().GetBytes("205");
            byte[] rcv = new byte[3];
            cl._clientStream.Write(buffer, 0, buffer.Length);
            cl._clientStream.Flush();
            cl._clientStream.Read(rcv, 0, 3);
            string answer = System.Text.Encoding.UTF8.GetString(rcv);
            if (answer != "106") /*failure*/ System.Windows.MessageBox.Show(this, "Error on getting the list of available rooms");
            else
            {
                rcv = new byte[4];
                cl._clientStream.Read(rcv, 0, 4);
                if (System.Text.Encoding.UTF8.GetString(rcv) == "0000")
                {
                    ListBoxItem item = new ListBoxItem();
                    item.Content = "There are no rooms available at the moment.";
                    roomsList.Items.Add(item);
                }
                else
                {
                    int questionNum = Int32.Parse(System.Text.Encoding.UTF8.GetString(rcv));
                    for (int i = 0; i < questionNum; i++)
                    {
                        ListBoxItem item = new ListBoxItem();
                        rcv = new byte[4];
                        cl._clientStream.Read(rcv, 0, 4);
                        item.Content = "room " + System.Text.Encoding.UTF8.GetString(rcv) + ": ";
                        rcv = new byte[2];
                        cl._clientStream.Read(rcv, 0, 2);
                        int nameLen = Int32.Parse(System.Text.Encoding.UTF8.GetString(rcv));
                        rcv = new byte[nameLen];
                        cl._clientStream.Read(rcv, 0, nameLen);
                        item.Content += System.Text.Encoding.UTF8.GetString(rcv);
                        roomsList.Items.Add(item);
                    }
                }
            }
        }
        private void click_room(object sender, MouseButtonEventArgs e)
        {
            System.Windows.MessageBox.Show(this, "works");
        }
        connectedUser secondForm;

        private void click_connectedUsers(object sender, MouseButtonEventArgs e)
        {
            if (secondForm != null)
            {
                secondForm.Close();
                string id = (string)((ListBoxItem)sender).Content;
                secondForm = new connectedUser(id.Substring(5,4)); //extract the id from the string inserted to the room's listBoxItem
                //How ever you are passing information to the secondWindow
                secondForm.Show();
            }
            else
            {
                if (secondForm == null)
                {
                    string id = (string)((ListBoxItem)sender).Content;
                    secondForm = new connectedUser(id.Substring(5, 4)); //extract the id from the string inserted to the room's listBoxItem
                    secondForm.Show();
                }
                else
                    secondForm.Activate();
            }
        }
    }
}
