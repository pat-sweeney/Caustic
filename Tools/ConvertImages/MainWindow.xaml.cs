using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
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
using static System.Net.Mime.MediaTypeNames;

namespace ConvertImages
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        private string _currentFolderPath;
        private BitmapImage _collapsedImage;
        private BitmapImage _expandedImage;
        private BitmapImage _folderImage;
        private BitmapImage _pngImage;
        private BitmapImage _jpegImage;
        private BitmapImage _bmpImage;
        private BitmapImage _anyImage;
        private BitmapImage _diskImage;
        private BitmapImage _image;
        private Visibility _imageView = Visibility.Collapsed;
        private Visibility _folderView = Visibility.Visible;
        public Visibility FolderView
        {
            get { return _folderView; }
            set
            {
                _folderView = value;
                RaisePropertyChange("FolderView");
            }
        }
        public Visibility ImageView
        {
            get { return _imageView; }
            set
            {
                _imageView = value;
                RaisePropertyChange("ImageView");
            }
        }

        public event PropertyChangedEventHandler? PropertyChanged;
        public void RaisePropertyChange(string name)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(name));
            }
        }

        public BitmapImage Image { get { return _image; } set { _image = value; 
                RaisePropertyChange("Image"); } }

        private void ShowImage(string url)
        {
            try
            {
                BitmapImage bitmap = new BitmapImage();
                bitmap.BeginInit();
                bitmap.UriSource = new Uri(url, UriKind.Absolute);
                bitmap.EndInit();
                Image = bitmap;
                ImageView = Visibility.Visible;
                FolderView = Visibility.Collapsed;
            }
            catch { }
        }

        private void ShowFolderContents(string folderPath)
        {
            if (folderPath == null)
                return;
            ImageView = Visibility.Collapsed;
            FolderView = Visibility.Visible;
            _currentFolderPath = folderPath;
            DispImage.Visibility = Visibility.Collapsed;
            FolderGrid.Visibility = Visibility.Visible;
            FolderGrid.Children.Clear();
            IEnumerable<string> dirs;
            try
            {
                dirs = Directory.EnumerateDirectories(folderPath);
            }
            catch (Exception ex)
            {
                return;
            }
            foreach (var dir in dirs)
            {
                if (dir.Contains("$"))
                    continue;
                StackPanel sp = new StackPanel();
                sp.MouseDown += (o, e) =>
                {
                    if (Keyboard.GetKeyStates(Key.LeftCtrl) != KeyStates.Down)
                    {
                        foreach (var child in FolderGrid.Children)
                        {
                            (child as StackPanel).Background = Brushes.White;
                        }
                    }
                    sp.Background = (sp.Background == Brushes.LightSteelBlue) ? Brushes.White : Brushes.LightSteelBlue;
                };
                sp.Background = Brushes.White;
                sp.Width = 125;
                sp.Orientation = Orientation.Vertical;
                var img = new System.Windows.Controls.Image();
                img.Source = _folderImage;
                img.Width = 100;
                img.Height = 100;
                sp.Children.Add(img);
                TextBlock tb = new TextBlock();
                tb.Text = System.IO.Path.GetFileName(dir);
                tb.Margin = new Thickness(5, 0, 0, 0);
                sp.Children.Add(tb);
                FolderGrid.Children.Add(sp);
            }
            var files = Directory.EnumerateFiles(folderPath);
            foreach (var file in files)
            {
                string ext = System.IO.Path.GetExtension(file);
                string filterMask;
                switch (SourceFilterCB.SelectedIndex)
                {
                    case 0: filterMask = ".png"; break;
                    case 1: filterMask = ".bmp"; break;
                    case 2: filterMask = ".jpg;.jpeg"; break;
                    case 3: filterMask = ".heic"; break;
                    case 4: filterMask = ".mov"; break;
                    case 5: filterMask = ".mp4"; break;
                    default: filterMask = ".png;.bmp;.jpg;.jpeg;.heic;.mov;.mp4"; break;
                }
                BitmapImage bmi;
                ext = ext.ToLower();
                if (!filterMask.Contains(ext))
                    continue;
                if (ext.ToLower() == ".bmp")
                {
                    bmi = _bmpImage;
                }
                else if (ext.ToLower() == ".jpg" || ext.ToLower() == ".jpeg")
                {
                    bmi = _jpegImage;
                }
                else if (ext.ToLower() == ".png")
                {
                    bmi = _pngImage;
                }
                else
                {
                    bmi = _anyImage;
                }
                var sp = new StackPanel();
                sp.Width = 125;
                sp.Orientation = Orientation.Vertical;
                var img = new System.Windows.Controls.Image();
                img.Source = bmi;
                img.Width = 100;
                img.Height = 100;
                sp.Children.Add(img);
                var tb = new TextBlock();
                tb.Text = System.IO.Path.GetFileName(file);
                tb.Margin = new Thickness(5, 0, 0, 0);
                sp.Children.Add(tb);
                FolderGrid.Children.Add(sp);
            }
        }

        public void BuildFileTree()
        {
            DriveInfo[] allDrives = DriveInfo.GetDrives();
            foreach (var drive in allDrives)
            {
                AddItem(drive.Name, drive.Name, _diskImage, FileTree.Items);
            }
        }
        
        private void AddItem(string root, string label, BitmapImage icon, ItemCollection parent)
        {
            TreeViewItem item = new TreeViewItem();
            StackPanel sp = new StackPanel();
            sp.Orientation = Orientation.Horizontal;
            item.IsExpanded = false;
            item.Expanded += (o, e) =>
            {
                Type t = (item.Items[0] as TreeViewItem).Header.GetType();
                if (item.Items.Count == 1 && t == typeof(string))
                {
                    item.Items.Clear();
                    FillTree(root, item.Items);
                }
            };
            var img = new System.Windows.Controls.Image();
            img.Source = icon;
            img.Width = 16;
            img.Height = 16;
            img.MouseDown += (o, e) =>
            {
                ShowFolderContents(root);
            };
            sp.Children.Add(img);
            TextBlock tb = new TextBlock();
            tb.Text = label;
            tb.Margin = new Thickness(5, 0, 0, 0);
            tb.MouseDown += (o, e) =>
            {
                ShowFolderContents(root);
            };
            sp.Children.Add(tb);
            item.Header = sp;

            // Add single item
            TreeViewItem itemchild = new TreeViewItem();
            itemchild.Header = ".";
            item.Items.Add(itemchild);

            parent.Add(item);
        }
        
        public void FillTree(string root, ItemCollection parent)
        {
            IEnumerable<string> dirs;
            try
            {
                dirs = Directory.EnumerateDirectories(root);
            }
            catch (Exception ex)
            {
                return;
            }
            foreach (var dir in dirs)
            {
                if (dir.Contains("$"))
                    continue;
                AddItem(dir, System.IO.Path.GetFileName(dir), _folderImage, parent);
            }
            var files = Directory.EnumerateFiles(root);
            foreach (var file in files)
            {
                var childitem = new TreeViewItem();
                string ext = System.IO.Path.GetExtension(file);
                BitmapImage bmi;
                if (ext.ToLower() == ".bmp")
                {
                    bmi = _bmpImage;
                }
                else if (ext.ToLower() == ".jpg" || ext.ToLower() == ".jpeg")
                {
                    bmi = _jpegImage;
                }
                else if (ext.ToLower() == ".png")
                {
                    bmi = _pngImage;
                }
                else
                {
                    bmi = _anyImage;
                }
                var sp = new StackPanel();
                sp.Orientation = Orientation.Horizontal;
                var img = new System.Windows.Controls.Image();
                img.Source = bmi;
                img.Width = 12;
                img.Height = 12;
                sp.Children.Add(img);
                img.MouseDown += (o, e) =>
                {
                    ShowImage(file);
                };
                var tb = new TextBlock();
                tb.Text = System.IO.Path.GetFileName(file);
                tb.Margin = new Thickness(5, 0, 0, 0);
                tb.MouseDown += (o, e) =>
                {
                    ShowImage(file);
                };
                sp.Children.Add(tb);
                childitem.Header = sp;
                parent.Add(childitem);
            }
        }

        private BitmapImage LoadImage(string filename)
        {
            var bmi = new BitmapImage();
            bmi.BeginInit();
            bmi.UriSource = new Uri("pack://application:,,,/Resources/" + filename);
            bmi.EndInit();
            return bmi;
        }

        public MainWindow()
        {
            this.DataContext = this;
            InitializeComponent();
            _collapsedImage = LoadImage("Collapsed.png");
            _expandedImage = LoadImage("Expanded.png");
            _folderImage = LoadImage("folder.png");
            _pngImage = LoadImage("png.png");
            _jpegImage = LoadImage("jpeg.png");
            _bmpImage = LoadImage("bmp.png");
            _anyImage = LoadImage("any.png");
            _diskImage = LoadImage("disk.png");
            BuildFileTree();
        }

        private void SourceFilterCB_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            ShowFolderContents(_currentFolderPath);
        }
    }
}
