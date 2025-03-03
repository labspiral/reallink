using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using SharedModels;
using SpiralLab.RealLink;

namespace ConsoleApp
{
    internal static class Program
    {
        static SpiralLab.RealLink.Client client = null;

       
        static async Task Main()
        {
            client = new SpiralLab.RealLink.Client("http://localhost:5000", "reallink1", "consoleapp");          
            RegisterCallBacks();

            long no = 0;
            do
            {
                Console.WriteLine("Demo for RealLink c# console client - (c)SpiralLAB");
                Console.WriteLine("'1' : start");
                Console.WriteLine("'2' : reconnect");
                Console.WriteLine("'3' : send");
                Console.WriteLine("'4' : send(all)");
                Console.WriteLine("'5' : request");
                Console.WriteLine("'6' : stop");
                Console.WriteLine("'Q' : quit");
                Console.Write("Select your target : ");
                ConsoleKeyInfo key = Console.ReadKey(false);
                if (key.Key == ConsoleKey.Q)
                    break;
                Console.WriteLine($"{Environment.NewLine}");
                switch (key.Key)
                {
                    case ConsoleKey.D1:
                        Console.WriteLine("Starting Reallink Client 'c# console app' is running and try to connect");
                        try
                        {
                            await client.StartAsync();
                        }
                        catch (Exception ex)
                        {
                            Console.WriteLine(ex.ToString());
                        }
                        break;
                    case ConsoleKey.D2:
                        Console.WriteLine("Reconnecting Reallink ...");
                        try
                        {
                            await client.ReconnectAsync();
                        }
                        catch (Exception ex)
                        {
                            Console.WriteLine(ex.ToString());
                        }
                        break;
                    case ConsoleKey.D3:
                        {
                            var substrate = new Substrate();
                            substrate.Name = $"CREATED FROM CONSOLE {no++}";
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
                                bool result = await client.InvokeAsync<bool>("Send", "wpfapp", "substrate", substrate);
                            }
                            catch (Exception ex)
                            {
                                Console.WriteLine(ex.ToString());
                            }
                        }
                        break;
                    case ConsoleKey.D4:
                        {
                            var substrate = new Substrate();
                            substrate.Name = $"CREATED FROM CONSOLE {no++}";
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
                                bool result = await client.InvokeAsync<bool>("Send", "all", "substrate", substrate);
                            }
                            catch (Exception ex)
                            {
                                Console.WriteLine(ex.ToString());
                            }
                        }
                        break;
                    case ConsoleKey.D5:
                        try
                        {
                            string name = $"CREATED FROM CONSOLE APP {no}";
                            var substrate = await client.InvokeAsync<Substrate>("Request", "wpfapp", "substrate", name);
                            // ...
                        }
                        catch (Exception ex)
                        {
                            Console.WriteLine(ex.ToString());
                        }
                        break;
                    case ConsoleKey.D6:
                        try
                        {
                            await client.StopAsync();
                        }
                        catch (Exception ex)
                        {
                            Console.WriteLine(ex.ToString());
                        }
                        break;
                }
            } while (true);
            client?.Dispose();
        }

        static void RegisterCallBacks()
        {
            client.Closed += (exception) =>
            {
                if (exception == null)
                    Console.WriteLine($"{Environment.NewLine}{DateTime.Now} Connection has closed");
                else
                    Console.WriteLine($"{Environment.NewLine}{DateTime.Now} Connection has closed due to an error: {exception.Message}");
                return Task.CompletedTask;
            };

            client.On("Receive",
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
                            Console.WriteLine($"{Environment.NewLine}{DateTime.Now} Receive: {from} {message} {substrate?.ToString()}");
                            return Task.CompletedTask;
                        default:
                            return Task.FromException(new Exception($"Invalid message format: {message}"));
                    }
                });


            client.On("Response",
                new[] { typeof(string), typeof(string), typeof(object) },
                async (args, state) =>
                {
                    string from = (string)args[0];
                    string message = (string)args[1];
                    object arg = args[2];
                    Console.WriteLine($"{Environment.NewLine}{DateTime.Now} Response: {from} {message}");
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

    }
}
