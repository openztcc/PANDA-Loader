using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Data.Core;
using Avalonia.Data.Core.Plugins;
using Avalonia.Markup.Xaml;
using PandaLdr.ViewModels;
using PandaLdr.Views;
using PandaLdr.Services;

namespace PandaLdr
{
    public partial class App : Application
    {
        private string _iniPath = "C:\\Program Files (x86)\\Microsoft Games\\Zoo Tycoon\\zoo.ini";
        private ISettingsService _settingsService;

        public override void Initialize()
        {
            AvaloniaXamlLoader.Load(this);
        }

        public override void OnFrameworkInitializationCompleted()
        {
            if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
            {
                // Line below is needed to remove Avalonia data validation.
                // Without this line you will get duplicate validations from both Avalonia and CT
                BindingPlugins.DataValidators.RemoveAt(0);
                _settingsService = new IniService(_iniPath);
                desktop.MainWindow = new MainWindow
                {
                    DataContext = new MainWindowViewModel(_settingsService),
                };
            }

            base.OnFrameworkInitializationCompleted();
        }
    }
}