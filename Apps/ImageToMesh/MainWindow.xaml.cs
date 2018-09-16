//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
using System.Windows;
using System.Windows.Media.Imaging;

namespace ImageToMesh
{
    public partial class MainWindow : Window
    {
        private CausticInterop.Image image = null;
        private WriteableBitmap wbm = null;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void LoadImageClick(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();
            if (dlg.ShowDialog() == true)
            {
                this.image = new CausticInterop.Image();
                this.wbm = this.image.Load(dlg.FileName);
                DispImage.Source = this.wbm;
                ImageWidth.Text = $"{this.wbm.Width}";
                ImageHeight.Text = $"{this.wbm.Height}";
            }
        }

        private void SaveMeshClick(object sender, RoutedEventArgs e)
        {
        }

        private void BuildMeshClick(object sender, RoutedEventArgs e)
        {
            this.image.ExtractMesh();
        }
    }
}
