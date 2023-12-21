using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace PandaLdr.ViewModels
{
    public class MainWindowViewModel : ViewModelBase
    {
#pragma warning disable CA1822 // Mark members as static
        public string Greeting => "Welcome to Avalonia!";

        public ObservableCollection<string> MyList { get; set; } = new ObservableCollection<string> { "Item 1", "Item 2", "Item 3" };

        //public ObservableCollection<string> MyList
        //{
        //    get => _myList;
        //    set
        //    {
        //        _myList = value;
        //    }
        //}

        public MainWindowViewModel()
        {
        }


    }
}