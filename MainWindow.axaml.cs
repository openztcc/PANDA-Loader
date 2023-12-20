using Avalonia.Controls;
using Avalonia.Markup.Xaml;

namespace PandaLdr
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void InitializeComponent()
        {
            AvaloniaXamlLoader.Load(this);
        }

        private void Button_Click(object sender, Avalonia.Interactivity.RoutedEventArgs e)
        {

        }
    }
}