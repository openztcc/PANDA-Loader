using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows.Input;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;

namespace PandaLdr.ViewModels
{
    public partial class MainWindowViewModel : ObservableObject
    {
        private object _currentView;

        public object CurrentView
        {
            get => _currentView;
            set
            {
                _currentView = value;
                OnPropertyChanged(nameof(CurrentView));
            }
        }

        public ICommand NavigateHomeCommand { get; }
        public ICommand NavigateSettingsCommand { get; }

        public MainWindowViewModel()
        {
            // Initialize commands
            NavigateHomeCommand = new RelayCommand(NavigateHome);
            NavigateSettingsCommand = new RelayCommand(NavigateSettings);

            // Set default view
            CurrentView = new HomeViewModel();
        }

        private void NavigateHome()
        {
            CurrentView = new HomeViewModel();
        }

        private void NavigateSettings()
        {
            CurrentView = new SettingsViewModel();
        }

        // Implement INotifyPropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged(string propertyName) =>
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }
}
