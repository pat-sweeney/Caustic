using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace FoodInventory
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public bool DownloadInfo { get; set; }
        public string Filename { get; set; }

        public App()
        {
            this.Startup += new StartupEventHandler(Application_Startup);
        }

        private void Application_Startup(object sender, StartupEventArgs e)
        {
            Filename = @"d:\Food\Food.csv";
            for (int i = 0; i < e.Args.Length; i++)
            {
                if (e.Args[i] == "-download")
                    DownloadInfo = true;
                else if (e.Args[i] == "-input")
                    Filename = e.Args[++i];
            }
        }
    }
}
