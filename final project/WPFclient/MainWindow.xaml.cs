﻿using System;
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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Net.Sockets;
using System.Net;

namespace WPFclient
{         
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        ClientBody cl;
        public MainWindow()
        {
            cl = (ClientBody)WPFclient.App.Current.Properties["client"];
            //https://stackoverflow.com/questions/15657637/condition-variables-c-net //read on CV
            InitializeComponent();
            WindowStartupLocation = WindowStartupLocation.CenterScreen;
        }

        private void click_mainMenu(object sender, RoutedEventArgs e)
        {
            try
            {
                int i;
                string usernameLen = (username.Text.Length).ToString().PadLeft(2, '0'), passLen = (pass.Text.Length).ToString().PadLeft(2, '0');
                byte[] buffer = new ASCIIEncoding().GetBytes("200" + usernameLen + username.Text + passLen + pass.Text), rcv = new byte[4];
                cl._clientStream.Write(buffer, 0, buffer.Length);
                cl._clientStream.Flush();
                cl._clientStream.Read(rcv, 0, 4);
                string answer = System.Text.Encoding.UTF8.GetString(rcv);
                if (answer == "1020" || answer == "1023"){ //success
                    if (answer == "1023") /*Successfuly signed in as admin*/ WPFclient.App.Current.Properties["isAdmin"] = true;
                    else WPFclient.App.Current.Properties["isAdmin"] = false;
                    MessageBox.Show(this, "Sign in succesful, you are connected, " + username.Text + "!");
                    cl._username = username.Text;
                    mainMenu m = new mainMenu(); //create mainMenu and open it
                    m.Show();
                    this.Close();
                }
                else if (answer == "1021") /*wrong details*/ MessageBox.Show(this, "Sign in unsuccesful, wrong name or password");
                else if (answer == "1022") /*already connected*/ MessageBox.Show(this, "Sign in unsuccesful, this user is already connected");
            }
            catch (Exception ex)
            {
                MessageBox.Show(this, ex.Message);
            }
        }
        private void click_signUp(object sender, RoutedEventArgs e)
        {
            int i;
            for (i = 0; i < WPFclient.App.Current.Windows.Count; i++) if (WPFclient.App.Current.Windows[i].ToString() == "WPFclient.signUp") break;
            if (i == WPFclient.App.Current.Windows.Count) //if there is signUp open already
            {
                signUp m = new signUp(); //else create one and open it
                m.Show();
            }
            else WPFclient.App.Current.Windows[i].Show();
            this.Close();
        }
        public void TextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            if (tb.Text == "UserName" || tb.Text == "Password")
            {
                tb.Text = string.Empty;
            }
        }
        private void TextBox_LostFocus_username(object sender, RoutedEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            string defaultText = "UserName";
            tb.Text = tb.Text == string.Empty ? defaultText : tb.Text;
        }
        private void TextBox_LostFocus_password(object sender, RoutedEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            string defaultText = "Password";
            tb.Text = tb.Text == string.Empty ? defaultText : tb.Text;
        }
    }
    public class ClientBody
    {
        public TcpClient _client;
        public NetworkStream _clientStream;
        public string _username;
        public bool _isConnected;
        public ClientBody()
        {
            _client = new TcpClient();
            try
            {
                _client.Connect("127.0.0.1", 8820);
                _clientStream = _client.GetStream();
                _isConnected = true;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
    }
}