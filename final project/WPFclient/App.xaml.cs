using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using System.Text;

namespace WPFclient
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public void App_Startup(object sender, StartupEventArgs e)
        {
            this.Properties["client"] = new ClientBody();       
        }
        protected override void OnExit(ExitEventArgs e)
        {
            //do your things
            byte[] buffer = new ASCIIEncoding().GetBytes("299");//when the window is closed, send the exit code
            ((ClientBody)this.Properties["client"])._clientStream.Write(buffer, 0, buffer.Length);
            ((ClientBody)this.Properties["client"])._clientStream.Flush();
            base.OnExit(e);
        }
    }
}
