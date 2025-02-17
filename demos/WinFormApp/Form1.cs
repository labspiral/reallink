using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using System.Windows.Forms;
using SharedModels;
using SpiralLab.RealLink;

namespace WindowsFormsApp
{
    public partial class Form1 : Form
    {
        
        Client _client = null;

        public Form1()
        {
            InitializeComponent();
            _client = new SpiralLab.RealLink.Client("http://localhost:5000", "reallink1", "winformapp");
            RegisterCallBacks();

        }

        private void AddLogMessage(string message)
        {
            this.BeginInvoke(new MethodInvoker(delegate
            {
                this.listBox1.Items.Add($"{DateTime.Now} {message}");
            }));
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            try
            {
                _client?.StopAsync();
            }
            catch (Exception ex)
            {
                AddLogMessage($"{ex.Message}");
            }
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
                   string userName = (string)args[0];
                   string message = (string)args[1];
                   object arg = args[2];
                   switch (message)
                   {
                       case "substrate":
                           var substrate = Helper.Deserialize<Substrate>(arg);
                           AddLogMessage($"Received: {userName}, {message}, {substrate?.ToString()}");
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

        private async void btnStart_Click(object sender, EventArgs e)
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

        private async void btnReconnect_Click(object sender, EventArgs e)
        {
            AddLogMessage("reallink is reconnecting...");
            try
            {
                await _client?.ReconnectAsync();
            }
            catch (Exception ex)
            {
                AddLogMessage($"{ex.Message}");
            }
        }

        long no = 0;
        private async void btnSend_Click(object sender, EventArgs e)
        {
            var substrate = new Substrate();
            substrate.Name = $"CREATED FROM WINFORM APP {no++}";
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
                bool result = await _client.InvokeAsync<bool>("Send", "cppapp", "substrate", substrate);
            }
            catch (Exception ex)
            {
                AddLogMessage($"{ex.Message}");
            }
        }
        private async void btnRequest_Click(object sender, EventArgs e)
        {
            try
            {
                // c++ client is not supported 'Request' method
                //
                //string name = $"CREATED FROM WINFORM APP {no}";
                //var substrate = await _client.InvokeAsync<Substrate>("Request", "cppapp", "substrate", name);
                //AddLogMessage($"Request: {substrate}");
            }
            catch (Exception ex)
            {
                AddLogMessage($"{ex.Message}");
            }
        }
        private async void btnStop_Click(object sender, EventArgs e)
        {
            AddLogMessage($"reallink is stopping");
            try
            {
                await _client?.StopAsync();
            }
            catch (Exception ex)
            {
                AddLogMessage($"{ex.Message}");
            }
        }

    }
}
