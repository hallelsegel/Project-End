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
            WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;
            this.Background = new ImageBrush(new BitmapImage(new Uri(@"pack://application:,,,/WPFclient;component/Resources/BG.png")));
            
        }
        private void click_mainMenu(object sender, RoutedEventArgs e)
        {
            mainMenu window1 = new mainMenu();
            window1.Show();
            this.Close();
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
                /*this.Background = new ImageBrush(new BitmapImage(new Uri(@"pack://application:,,,/WPFclient;component/Resources/BG_green.png")));
                await Task.Delay(1000);
                this.Background = new ImageBrush(new BitmapImage(new Uri(@"pack://application:,,,/WPFclient;component/Resources/BG.png")));*/
                /*correctAnswer window = new correctAnswer();
                window.Show();
                this.Close();*/
            }
            else if (answer == false)
            {
                imageIncorrect.Visibility = Visibility.Visible;
                textIncorrect.Visibility = Visibility.Visible;
                await Task.Delay(500);
                imageIncorrect.Visibility = Visibility.Hidden;
                textIncorrect.Visibility = Visibility.Hidden;
                /*this.Background = new ImageBrush(new BitmapImage(new Uri(@"pack://application:,,,/WPFclient;component/Resources/BG_red.png")));
                await Task.Delay(1000);
                this.Background = new ImageBrush(new BitmapImage(new Uri(@"pack://application:,,,/WPFclient;component/Resources/BG.png")));*/
                /*wrongAnswer window = new wrongAnswer();
                window.Show();
                this.Close();*/
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
