using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows.Input;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;

namespace PandaLdr.ViewModels
{
    internal class HomeViewModel : ObservableObject
    {
        private object _homeView;

        public object HomeView
        {
            get => _homeView;
            set
            {
                _homeView = value;
                OnPropertyChanged(nameof(HomeViewModel));
            }
        }

        public HomeViewModel()
        {
            HomeView = new HomeViewModel();
        }
    }
}
