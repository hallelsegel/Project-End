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
using System.Threading;
using System.ComponentModel;

namespace WPFclient
{
    /// <summary>
    /// Interaction logic for inRoom.xaml
    /// </summary>
    public partial class inRoom : Window
    {
        bool _inRoom = true;
        bool isAdmin;
        ClientBody cl; //shared class 
        WindowCollection windows;
        private BackgroundWorker background_worker = new BackgroundWorker();
        public inRoom(bool _isAdmin, int _questionsNum, int _time, int _playersNum, string _roomName)
        {
            windows = WPFclient.App.Current.Windows; //for waitForMsg
            isAdmin = _isAdmin;
            cl = (ClientBody)WPFclient.App.Current.Properties["client"];
            InitializeComponent();
            questionNum.Content = questionNum.Content.ToString() + _questionsNum;
            time.Content = time.Content.ToString() + _time;
            players.Content = players.Content.ToString() + _playersNum;
            room.Content = room.Content.ToString() + _roomName;
            UserName.Content = cl._username;

            ListBoxItem user = new ListBoxItem();
            user.Content = cl._username;
            usersList.Items.Add(user);
            if (!isAdmin)
            {//hide the start game and close game buttons, as they aren't allowed for a non admin, and show the leave game button, and instructions
                //instead of the number of players, that is intended only for admins.
                players.Content = "Please wait for the admin to start the game";
                adminExitButton.Visibility= Visibility.Collapsed;
                adminStartButton.Visibility = Visibility.Collapsed;
                joinerExit.Visibility = Visibility.Visible;
            }
            WindowStartupLocation = WindowStartupLocation.CenterScreen;

            background_worker.WorkerSupportsCancellation = true;
            background_worker.WorkerReportsProgress = true;
            background_worker.DoWork += background_worker_DoWork;
            background_worker.ProgressChanged += background_worker_ProgressChanged;

            background_worker.RunWorkerAsync(this);
        }
        private void click_game(object sender, RoutedEventArgs e)
        {//open a game
            byte[] buffer = new ASCIIEncoding().GetBytes("217"); //217 == start game
            cl._clientStream.Write(buffer, 0, buffer.Length); //send message code to server
            cl._clientStream.Flush();
        }
        private void click_mainMenu(object sender, RoutedEventArgs e)
        {//either the "leave room" or the "close room" were pressed
            byte[] buffer;
            if (isAdmin) buffer = new ASCIIEncoding().GetBytes("215");
            else buffer = new ASCIIEncoding().GetBytes("211");
            cl._clientStream.Write(buffer, 0, buffer.Length); //send message code to server (closeRoom if admin and leaveRoom if not)
            cl._clientStream.Flush();
            //wait for the recieving function to get the answer and move to mainMenu according to that
        }
        void background_worker_DoWork(object sender, DoWorkEventArgs e)
        {
            while (_inRoom)
            {
                byte[] rcv = new byte[3];
                cl._clientStream.Read(rcv, 0, rcv.Length);
                string answer = System.Text.Encoding.UTF8.GetString(rcv, 0, rcv.Length);
                if (answer == "108") //108 == users in room answer from server (this means someone joined and the list should be updated)
                {background_worker.ReportProgress(108, sender);
                    Thread.Sleep(500);}
                else if ((answer == "112" && !isAdmin) || answer == "116") //112 == answer to leaveRoom from server, 116 == admin has closed the room
                {
                    if (answer == "112") cl._clientStream.Read(rcv, 0, 1); //another byte is send to indicate success or failure of leave room
                    string success = Encoding.UTF8.GetString(rcv, 0, 1);
                    if (answer == "116" || success == "0") //only leave room needs a success flag (0 is success)
                    {
                        background_worker.ReportProgress(116, sender); //open main menu in a thread that can
                        Thread.Sleep(500);
                    }
                }
                else if (answer == "118") //118 == game started, first question was send
                {
                    background_worker.ReportProgress(118, sender); //open main menu in a thread that can
                    Thread.Sleep(500);
                }
            }
        }
        void background_worker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {/* Same code as in connectedUser, but doesn't send for it, beacuse it is sent automatically when someone joins*/
            if (e.ProgressPercentage.CompareTo(108) == 0)
            {
                usersList.Items.Clear();
                byte[] rcv = new byte[1]; //0 if failed, otherwise number of users
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
            else if (e.ProgressPercentage.CompareTo(116) == 0)
            {
                int i;
                for (i = 0; i < windows.Count; i++) if (windows[i].ToString() == "WPFclient.mainMenu") break;
                if (i == windows.Count) //if there is mainMenu open already
                {
                    mainMenu m = new mainMenu(); //else create one and open it
                    m.Show();
                }
                else windows[i].Show();
                this.Close();
                _inRoom = false;
            }
            else if (e.ProgressPercentage.CompareTo(118) == 0)
            {
                int i;
                for (i = 0; i < windows.Count; i++) if (windows[i].ToString() == "WPFclient.game") break;
                if (i == windows.Count) //if there is mainMenu open already
                {
                    int timeParam = Int32.Parse(time.Content.ToString().Substring(20, time.Content.ToString().Length-20));
                    int questionParam = Int32.Parse(questionNum.Content.ToString().Substring(22, questionNum.Content.ToString().Length - 22));
                    game g = new game(timeParam, questionParam); //else create one and open it
                    g.Show();
                }
                else windows[i].Show();
                this.Close();
                _inRoom = false;
            }
        }
    }
}
