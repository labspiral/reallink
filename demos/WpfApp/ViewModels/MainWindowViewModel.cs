using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using SharedModels;
using SpiralLab.RealLink;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.ComponentModel;
using System.Diagnostics;


namespace WpfApp.ViewModels
{
    internal class MainWindowViewModel : ObservableObject
    {
        public ObservableCollection<string> LogItems
        {
            get =>  _logItems;
            set => SetProperty(ref _logItems, value);
        }

        public ICommand StartCommand => _startCommand ??= new RelayCommand(StartButtonClick);
        public ICommand ReconnectCommand => _reconnectCommand ??= new RelayCommand(ReconnectButtonClick);
        public ICommand SendCommand => _sendCommand ??= new RelayCommand(SendButtonClick);
        public ICommand RequestCommand => requestCommand ??= new RelayCommand(RequestButtonClick);
        public ICommand StopCommand => _stopCommand ??= new RelayCommand(StopButtonClick);
        public RelayCommand _startCommand;
        public RelayCommand _reconnectCommand;
        public RelayCommand _sendCommand;
        public RelayCommand requestCommand;
        public RelayCommand _stopCommand;

        private ObservableCollection<string> _logItems;
        private SpiralLab.RealLink.Client _client;


        public MainWindowViewModel(SpiralLab.RealLink.Client client)
        {
            _client = client;
            RegisterCallBacks();

            LogItems = new ObservableCollection<string>();
        }

        private void AddLogMessage(string message)
        {
            Application.Current?.Dispatcher?.BeginInvoke(() =>
            {
                LogItems.Add($"{DateTime.Now} {message}");
                OnPropertyChanged(nameof(LogItems));
            });
        }

        private void RegisterCallBacks()
        {
            _client.Closed += (exception) =>
            {
                if (exception == null)
                    AddLogMessage($"Connection has closed");
                else
                    AddLogMessage($"Connection has closed due to an error: {exception.Message}");
                return Task.CompletedTask;
            };
            _client.On("Receive",
                new[] { typeof(string), typeof(string), typeof(object) },
                (args, state) =>
                {
                    string from = (string)args[0];
                    string message = (string)args[1];
                    object arg = args[2];
                    switch (message)
                    {
                        case "substrate":
                            var substrate = Helper.Deserialize<Substrate>(arg);
                            AddLogMessage($"Received: {from}, {message}, {substrate?.ToString()}");
                            return Task.CompletedTask;
                        default:
                            return Task.FromException(new Exception($"Invalid message format: {message}"));
                    }
                });

            _client.On("Response",
                new[] { typeof(string), typeof(string), typeof(object) },
                async (args, state) =>
                {
                    string from = (string)args[0];
                    string message = (string)args[1];
                    object arg = args[2];
                    AddLogMessage($"Response: {from} {message}");
                    switch (message)
                    {
                        case "substrate":
                            var substrate = new Substrate();
                            var name = Helper.Deserialize<string>(args[2]);
                            substrate.Name = name;
                            var rnd = new Random();
                            for (int i = 0; i < 4000; i++)
                            {
                                int col = rnd.Next();
                                int row = rnd.Next();
                                int bin = rnd.Next() % 10;
                                var unit = new Unit(col, row, bin);
                                substrate.Rows = rnd.Next();
                                substrate.Cols = rnd.Next();
                                substrate.Units.Add(unit);
                            }
                            return await Task.FromResult(substrate);
                        default:
                            return Task.FromException(new Exception($"Invalid message format: {message}"));
                    }
                });
        }

        private async void StartButtonClick()
        {
            AddLogMessage("reallink is starting ...");
            Debug.Assert(_client != null);
            try
            {
                await _client.StartAsync();
            }
            catch (Exception ex)
            {
                AddLogMessage($"{ex.Message}");
            }
        }

        private async void ReconnectButtonClick()
        {
            AddLogMessage("reallink is reconnecting...");
            try
            {
                await _client.ReconnectAsync();
            }
            catch (Exception ex)
            {
                AddLogMessage($"{ex.Message}");
            }
        }

        long no = 0;
        private async void SendButtonClick()
        {
            var substrate = new Substrate();
            substrate.Name = $"CREATED FROM WPF APP {no++}";
            var rnd = new Random();
            for (int i = 0; i < 4000; i++)
            {
                int col = rnd.Next();
                int row = rnd.Next();
                int bin = rnd.Next() % 10;
                var unit = new Unit(col, row, bin);
                substrate.Rows = rnd.Next();
                substrate.Cols = rnd.Next();
                substrate.Units.Add(unit);
            }
            try
            {
                bool result = await _client.InvokeAsync<bool>("Send", "winformapp", "substrate", substrate);
            }
            catch (Exception ex)
            {
                AddLogMessage(ex.Message);
            }
        }
        private async void RequestButtonClick()
        {
            try
            {
                string name = $"CREATED FROM WPF APP {no}";
                var substrate = await _client.InvokeAsync<Substrate>("Request", "winformapp", "substrate", name);
                AddLogMessage($"Request: {substrate}");
            }
            catch (Exception ex)
            {
                AddLogMessage(ex.Message);
            }
        }

        private async void StopButtonClick()
        {
            AddLogMessage("reallink is stopping");
            try
            {
                await _client.StopAsync();
            }
            catch (Exception ex)
            {
                AddLogMessage($"{ex.Message}");
            }
        }
    }
}
