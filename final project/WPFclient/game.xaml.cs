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
        int timeLeft, currQuestion, questionNum, timePerQuestion, corrects;
        List<string> answers = new List<string>();
        ClientBody cl; //shared class 
        public game(int _timePerQ, int _questionNum)
        {
            cl = (ClientBody)WPFclient.App.Current.Properties["client"];
            InitializeComponent();
            timePerQuestion = _timePerQ;
            questionNum = _questionNum;
            currQuestion = 0; corrects = 0;
            UserName.Content = cl._username;
            WindowStartupLocation = WindowStartupLocation.CenterScreen;
            this.Background = new ImageBrush(new BitmapImage(new Uri(@"pack://application:,,,/WPFclient;component/BG.png")));
            rcvQuestion();
        }
        private async void rcvQuestion()
        {
            byte[] rcv = new byte[3];
            score.Content = "Score: " + corrects + " / " + currQuestion;
            cl._clientStream.Read(rcv, 0, rcv.Length);
            int qLength = Int32.Parse(Encoding.UTF8.GetString(rcv, 0, rcv.Length));
            if (qLength == 0) MessageBox.Show(this, "Error in recieving question");
            else
            {
                rcv = new byte[qLength];
                cl._clientStream.Read(rcv, 0, rcv.Length);
                questionLabel.Text = Encoding.UTF8.GetString(rcv, 0, rcv.Length);
                answers.Clear();
                for (int i =0; i<4;i++)
                {
                    rcv = new byte[3];
                    cl._clientStream.Read(rcv, 0, rcv.Length);
                    int aLength = Int32.Parse(Encoding.UTF8.GetString(rcv, 0, rcv.Length));
                    rcv = new byte[aLength];
                    cl._clientStream.Read(rcv, 0, rcv.Length);
                    answers.Add(Encoding.UTF8.GetString(rcv, 0, rcv.Length));
                }
                button1.Content = answers[0];
                button2.Content = answers[1];
                button3.Content = answers[2];
                button4.Content = answers[3];
                timeLeft = timePerQuestion;
                timerLabel.Content = "Time left: " + timeLeft;
                currQLabel.Content = ++currQuestion + " / " + questionNum;
            }
            startTimer();
        }
        private async void startTimer()
        {
            while (timeLeft > 0)
            {
                timerLabel.Content = "Time left: " + timeLeft;
                timeLeft--;
                await Task.Delay(1000);
                timerLabel.Content = "Time left: " + timeLeft;
            }
            if (timeLeft == 0)
            {
                sendAnswer("2195" + timePerQuestion.ToString().PadLeft(2, '0'));
            }
        }
        private async void isCorrectScreen()
        {
            byte[] rcv = new byte[1]; // resize to recieve only one byte
            cl._clientStream.Read(rcv, 0, 1);
            string correct = System.Text.Encoding.UTF8.GetString(rcv);    
            if (correct == "1")
            { // If correct shows the correct pic 
                corrects++;
                imageCorrect.Visibility = Visibility.Visible;
                textCorrect.Visibility = Visibility.Visible;
                await Task.Delay(700);
                imageCorrect.Visibility = Visibility.Hidden;
                textCorrect.Visibility = Visibility.Hidden;
            }
            else
            { // If incorrect shows the incorrect pic 
                imageIncorrect.Visibility = Visibility.Visible;
                textIncorrect.Visibility = Visibility.Visible;
                await Task.Delay(700);
                imageIncorrect.Visibility = Visibility.Hidden;
                textIncorrect.Visibility = Visibility.Hidden;
            }
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
        private void click_checkAnswer(object sender, RoutedEventArgs e)
        {
            if (timeLeft > 0)
            {
                string name = ((Button)sender).Name;
                sendAnswer("219" + name[6] + (timePerQuestion - timeLeft).ToString().PadLeft(2, '0')); //use sendAnswer to send to the server the answer from the button to 
                timeLeft = -1;
            }
        }
        private async void sendAnswer(string answer)
        {
            byte[] buffer = new ASCIIEncoding().GetBytes(answer), rcv = new byte[3];
            //219 == send answer, name[6] == answer number (ex: "button1"), last bit == time taken to answer;
            cl._clientStream.Write(buffer, 0, buffer.Length); //send message code to server
            cl._clientStream.Flush();
            cl._clientStream.Read(rcv, 0, 3);
            string rcvMsg = System.Text.Encoding.UTF8.GetString(rcv);
            if (rcvMsg != "120") //120 = server answer
            { // Checks if recieved the correct message 
                MessageBox.Show(this, "Error in communication");
            }
            else
            {
                isCorrectScreen();
                await Task.Delay(300);
                rcv = new byte[3]; // resize to recieve only one byte
                cl._clientStream.Read(rcv, 0, 3);
                rcvMsg = System.Text.Encoding.UTF8.GetString(rcv);
                if (rcvMsg == "118") //118 = server send question
                {
                    timeLeft = timePerQuestion;
                    rcvQuestion(); //recieve next question
                }
                else if (rcvMsg == "121") //end game and results
                {
                    gameResults r = new gameResults(questionNum);
                    r.Show();
                    this.Close();
                }
            }
        }
    }
}
