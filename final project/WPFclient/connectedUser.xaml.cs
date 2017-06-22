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
    /// Interaction logic for connectedUser.xaml
    /// </summary>
    public partial class connectedUser : Window
    {
        ClientBody cl; //shared class 
        public connectedUser(string id)
        {
            cl = (ClientBody)WPFclient.App.Current.Properties["client"];
            InitializeComponent();
            UserName.Content = cl._username;
            //send the users in room request to server
            byte[] buffer = new ASCIIEncoding().GetBytes("207" + id);
            byte[] rcv = new byte[3];
            cl._clientStream.Write(buffer, 0, buffer.Length);
            cl._clientStream.Flush();
            cl._clientStream.Read(rcv, 0, 3);
            string answer = System.Text.Encoding.UTF8.GetString(rcv);
            if (answer != "108") /*failure*/ System.Windows.MessageBox.Show(this, "Error in getting users from room");
            else
            {
                rcv = new byte[1];
                cl._clientStream.Read(rcv, 0, 1);
                if (System.Text.Encoding.UTF8.GetString(rcv) == "0")
                {
                    ListBoxItem item = new ListBoxItem();
                    item.Content = "There are no users, the room probably closed";
                    usersList.Items.Add(item);
                }
                else
                {
                    int userNum = Int32.Parse(System.Text.Encoding.UTF8.GetString(rcv));
                    for (int i = 0; i < userNum; i++)
                    {
                        ListBoxItem item = new ListBoxItem();
                        rcv = new byte[2];
                        cl._clientStream.Read(rcv, 0, 2);
                        int nameLen = Int32.Parse(System.Text.Encoding.UTF8.GetString(rcv));
                        rcv = new byte[nameLen];
                        cl._clientStream.Read(rcv, 0, nameLen);
                        item.Content += System.Text.Encoding.UTF8.GetString(rcv);
                        usersList.Items.Add(item);
                    }
                }
            }
        }
    }
}
