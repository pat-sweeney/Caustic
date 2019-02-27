using System.Windows;
using System.Collections.Generic;
using System.IO;

namespace UPCReader
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            Dictionary<string, int> items = new Dictionary<string, int>();
            while (true)
            {
                System.Console.WriteLine("UPC:");
                string s = System.Console.ReadLine();
                if (s == "762274813938")
                    break;
                if (items.ContainsKey(s))
                    items[s]++;
                else
                    items.Add(s, 1);
            }
            // write data to csv
            FileStream fs = new FileStream(@"d:\inventory.csv", FileMode.Create);
            StreamWriter strm = new StreamWriter(fs);
            foreach (var data in items)
            {
                strm.WriteLine($"{data.Key},{data.Value.ToString()}");
            }
            strm.Flush();
            fs.Close();

#if false
            string connectionString = @"Data Source=Database.db;Version=3;FailIfMissing=True;Foreign Keys=True;";

            using (SQLiteConnection conn = new SQLiteConnection(connectionString))
            {
                conn.Open();
                string sql = "SELECT"
                conn.Close();
            }
                sqlite.
#endif
            this.DataContext = this;
            InitializeComponent();
        }
    }
}
