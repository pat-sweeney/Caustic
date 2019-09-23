//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
using System.Windows;
using System.Windows.Media.Imaging;

namespace ImageToMesh
{
    public partial class MainWindow : Window
    {
        private CausticInterop.Image image = null;
        private CausticInterop.Image image2 = null;
        private WriteableBitmap wbm = null;
        private WriteableBitmap wbm2 = null;

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
                this.image2 = this.image.Clone();
                this.wbm2 = this.image2.GetWritableBitmap();
                DispImage.Source = this.wbm;
                DispImage2.Source = this.wbm2;
                //ImageWidth.Text = $"{this.wbm.Width}";
                //ImageHeight.Text = $"{this.wbm.Height}";
            }
        }

        private void SaveMeshClick(object sender, RoutedEventArgs e)
        {
        }

        private void BuildMeshClick(object sender, RoutedEventArgs e)
        {
            this.image2.ExtractMesh();
        }
    }
}
