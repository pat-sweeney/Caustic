using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace WorldBuilder
{
    public partial class MainWindow : Window
    {
        public static RoutedCommand NewWorldCmd = new RoutedCommand();
        public static RoutedCommand LoadWorldCmd = new RoutedCommand();
        public static RoutedCommand SaveWorldCmd = new RoutedCommand();

        public MainWindow()
        {
            this.DataContext = this;
            InitializeComponent();
        }

        private void CanNewWorld(object sender, CanExecuteRoutedEventArgs e)
        {
            Control target = e.Source as Control;
            e.CanExecute = (target != null) ? true : false;
        }

        private void NewWorld(object sender, ExecutedRoutedEventArgs e)
        {
            MessageBox.Show("Custom Command Executed");
        }

        private void CanLoadWorld(object sender, CanExecuteRoutedEventArgs e)
        {
            Control target = e.Source as Control;
            e.CanExecute = (target != null) ? true : false;
        }

        private void LoadWorld(object sender, ExecutedRoutedEventArgs e)
        {
            MessageBox.Show("Custom Command Executed");
        }

        private void CanSaveWorld(object sender, CanExecuteRoutedEventArgs e)
        {
            Control target = e.Source as Control;
            e.CanExecute = (target != null) ? true : false;
        }

        private void SaveWorld(object sender, ExecutedRoutedEventArgs e)
        {
            MessageBox.Show("Custom Command Executed");
        }
    }
}
