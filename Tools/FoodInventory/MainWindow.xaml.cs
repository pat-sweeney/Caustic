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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
using System.Net;

namespace FoodInventory
{
    class MyWebClient : WebClient
    {
        protected override WebRequest GetWebRequest(Uri address)
        {
            HttpWebRequest request = base.GetWebRequest(address) as HttpWebRequest;
            request.AutomaticDecompression = DecompressionMethods.Deflate | DecompressionMethods.GZip;
            return request;
        }
    }
    
    /// <summary>
         /// Interaction logic for MainWindow.xaml
         /// </summary>
    public partial class MainWindow : Window
    {
        public void DownloadImages()
        {
            FileStream fs = new FileStream(@"d:\Food\Food.csv", FileMode.Open);
            StreamReader sr = new StreamReader(fs);
            while (!sr.EndOfStream)
            {
                var str = sr.ReadLine();
                var args = str.Split(new char[] { ',', ' ' });

                System.Console.WriteLine("Processing: " + args[0].ToString());

                string fn = @"d:\Food\Images\" + args[0] + ".png";
                if (!File.Exists(fn))
                {
                // Download product image
                    string url = "https://www.amazon.com/s?k="+ args[0] + "&x=0&y=0&ref=nb_sb_noss";
                    string result = "";
                    using (MyWebClient client = new MyWebClient())
                    {
                        client.Headers.Add("user-agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko)");
                        result = client.DownloadString(url);
                    }

                    if (result != "")
                    {
                        FileStream fs2 = new FileStream(@"d:\Food\Html\" + args[0] + ".html", FileMode.Create);
                        StreamWriter s2 = new StreamWriter(fs2);
                        s2.WriteLine(result);
                        s2.Flush();
                        fs2.Close();

                        int index = result.IndexOf("data-component-type=\"s-product-image\"");
                        if (index >= 0)
                        {
                            string substr = result.Substring(index, 1200);
                            int index2 = substr.IndexOf("<img src=\"");
                            if (index2 >= 0)
                            {
                                index2 += ("<img src=\"").Length;
                                string substr2 = substr.Substring(index2, 256);
                                index2 = 0;
                                while (substr2[index2] != '\"')
                                    index2++;
                                string imageurl = substr2.Substring(0, index2);

                                if (imageurl != null)
                                    using (var web = new WebClient())
                                    {
                                        web.DownloadFile(imageurl, fn);
                                    }
                                System.Threading.Thread.Sleep(3000);
                            }
                        }
                    }
                }
            }
            fs.Close();
        }

        void BuildUIList()
        {
            FileStream fs = new FileStream((Application.Current as App).Filename, FileMode.Open);
            StreamReader sr = new StreamReader(fs);
            int row = 0;
            while (!sr.EndOfStream)
            {
                var str = sr.ReadLine();
                var args = str.Split(new char[] { ',', ' ' });

                System.Console.WriteLine("Processing: " + args[0].ToString());

                TheGrid.RowDefinitions.Add(new RowDefinition());

                var label = new TextBox();
                label.IsReadOnly = true;
                label.Width = 200;
                label.SetValue(Grid.RowProperty, row);
                label.SetValue(Grid.ColumnProperty, 0);
                label.Text = args[0].ToString();
                TheGrid.Children.Add(label);

                string fn = @"d:\Food\Images\" + args[0] + ".png";
                bool imageExists = false;
                if (File.Exists(fn))
                    imageExists = true;
                else
                {
                    fn = @"d:\Food\Images\" + args[0] + ".jpg";
                    if (File.Exists(fn))
                        imageExists = true;
                    else
                    {
                        fn = @"d:\Food\Images\unknown.png";
                        if (File.Exists(fn))
                            imageExists = true;
                    }
                }
                if (imageExists)
                {
                    var image = new Image();
                    image.SetValue(Grid.RowProperty, row);
                    image.SetValue(Grid.ColumnProperty, 1);
                    BitmapImage bitmap = new BitmapImage();
                    bitmap.BeginInit();
                    bitmap.UriSource = new Uri(fn, UriKind.Absolute);
                    bitmap.EndInit();
                    image.Source = bitmap;
                    image.Width = 200;
                    image.Height = 200;
                    TheGrid.Children.Add(image);
                }

                var label2 = new Label();
                label2.Width = 200;
                label2.SetValue(Grid.RowProperty, row);
                label2.SetValue(Grid.ColumnProperty, 2);
                label2.Content = args[1].ToString();
                TheGrid.Children.Add(label2);

                row++;
            }
            fs.Close();
        }

        public MainWindow()
        {
            this.DataContext = this;
            InitializeComponent();
            if ((Application.Current as App).DownloadInfo)
                DownloadImages();
            BuildUIList();
        }
    }
}
