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
        
        Client client = null;

        public Form1()
        {
            InitializeComponent();
            client = new SpiralLab.RealLink.Client("http://localhost:5000", "reallink1", "winformapp");
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
                client?.StopAsync();
            }
            catch (Exception ex)
            {
                AddLogMessage($"{ex.Message}");
            }
        }

        private async void btnStart_Click(object sender, EventArgs e)
        {
            Debug.Assert(client != null);
            client.On("Receive",
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
                            break;
                    }
                    return Task.CompletedTask;
                });

            try
            {
                AddLogMessage("reallink is starting ...");
                await client.StartAsync();
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
                await client?.ReconnectAsync();
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
                bool result = await client.InvokeAsync<bool>("Send", "consoleapp", "substrate", substrate);
            }
            catch (Exception ex)
            {
                AddLogMessage($"{ex.Message}");
            }
        }


        private void btnStop_Click(object sender, EventArgs e)
        {
            AddLogMessage($"reallink is stopping");
            try
            {
                client?.StopAsync();
            }
            catch (Exception ex)
            {
                AddLogMessage($"{ex.Message}");
            }
        }

    }
}
