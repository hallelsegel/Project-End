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
using System.Windows.Shapes;

namespace WPFclient
{
    /// <summary>
    /// Interaction logic for game.xaml
    /// </summary>
    public partial class game : Window
    {
        ClientBody cl; //shared class 
        public game()
        {
            cl = (ClientBody)WPFclient.App.Current.Properties["client"];
            this.Closed += new EventHandler(theWindow_Closed);
            InitializeComponent();
            UserName.Content = cl._username;
            WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;
            this.Background = new ImageBrush(new BitmapImage(new Uri(@"pack://application:,,,/WPFclient;component/BG.png")));
            
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
        private async void click_checkAnswer(object sender, RoutedEventArgs e)
        {
            bool answer = true;// send to function that checks answer. answer = function return
            Random gen = new Random();
            int prob = gen.Next(100);
            if (prob < 50)
            {
                answer = true;
            }
            else
            {
                answer = false;
            }
            Button b = (Button)sender;
            if (answer == true)
            {
                imageCorrect.Visibility = Visibility.Visible;
                textCorrect.Visibility = Visibility.Visible;
                await Task.Delay(500);
                imageCorrect.Visibility = Visibility.Hidden;
                textCorrect.Visibility = Visibility.Hidden;
            }
            else if (answer == false)
            {
                imageIncorrect.Visibility = Visibility.Visible;
                textIncorrect.Visibility = Visibility.Visible;
                await Task.Delay(500);
                imageIncorrect.Visibility = Visibility.Hidden;
                textIncorrect.Visibility = Visibility.Hidden;
            }
        }
        private void theWindow_Closed(object sender, System.EventArgs e)
        {
            byte[] buffer = new ASCIIEncoding().GetBytes("299");//when the window is closed, send the exit code
            cl._clientStream.Write(buffer, 0, buffer.Length);
            cl._clientStream.Flush();
        }
    }
}
