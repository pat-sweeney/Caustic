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
                                System.Threading.Thread.Sleep(500);
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
            int col = 0;
            while (!sr.EndOfStream)
            {
                var str = sr.ReadLine();
                var args = str.Split(new char[] { ',', ' ' });

                System.Console.WriteLine("Processing: " + args[0].ToString());

                if (col == 5)
                {
                    TheGrid.RowDefinitions.Add(new RowDefinition());
                    col = 0;
                    row++;
                }

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

                var border = new Border();
                border.BorderThickness = new Thickness(3);
                border.BorderBrush = new SolidColorBrush(Colors.Black);
                border.SetValue(Grid.RowProperty, row);
                border.SetValue(Grid.ColumnProperty, col);
                TheGrid.Children.Add(border);

                var sp = new StackPanel();
                sp.Orientation = Orientation.Vertical;
                border.Child = sp;

                var label = new Label();
                label.Width = 200;
                label.Content = args[0].ToString() + " : " + args[1].ToString();
                label.FontSize = 24;
                label.Foreground = new SolidColorBrush(Color.FromArgb(255, 0, 0, 255));
                sp.Children.Add(label);

                if (imageExists)
                {
                    var image = new Image();
                    BitmapImage bitmap = new BitmapImage();
                    bitmap.BeginInit();
                    bitmap.UriSource = new Uri(fn, UriKind.Absolute);
                    bitmap.EndInit();
                    image.Source = bitmap;
                    image.Width = 200;
                    image.Height = 200;
                    sp.Children.Add(image);
                }
                col++;
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
