using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using PandaLdr.ViewModels;
using PandaLdr.Services;
using Avalonia.Svg.Commands;

namespace PandaLdr.ViewModels
{
    public partial class MainWindowViewModel : ViewModelBase
    {
        private readonly ISettingsService _settingsService;

        [ObservableProperty]
        private bool _isPaneOpen = true;

        [ObservableProperty]
        private ViewModelBase _currentPage = new HomeViewModel();

        [ObservableProperty]
        private ListItemTemplate? _selectedListItem;

        // Override the setter of the SelectedListItem property to create an instance of the selected ViewModel
        partial void OnSelectedListItemChanged(ListItemTemplate? value)
        {
            if (value is null) return;
            var instance = Activator.CreateInstance(value.ModelType);
            if (instance is not null)
            {
                CurrentPage = (ViewModelBase)instance;
            }
            else return;

            _settingsService = new IniService("settings.ini");
            SaveCommand = new RelayCommand(() => _settingsService.SaveIni(new IniData()));
        }

        public MainWindowViewModel()
        {
            _settingsService = new IniService("settings.ini");
            SaveCommand = new RelayCommand(() => _settingsService.SaveIni(new IniData()));
        }

        public ObservableCollection<ListItemTemplate> Items { get; } = new()
        { 
            new ListItemTemplate(typeof(HomeViewModel)),
            new ListItemTemplate(typeof(SettingsViewModel)),
        };

        [RelayCommand]
        public void TogglePane()
        {
            IsPaneOpen = !IsPaneOpen;
        }
    }

    public class ListItemTemplate
    {
        public ListItemTemplate(Type type)
        {
            ModelType = type;
            Label = type.Name.Replace("ViewModel", "");
        }

        public string Label { get; }
        public Type ModelType { get; }
    }
}
