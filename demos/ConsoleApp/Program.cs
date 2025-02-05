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
 

        static void Main()
        {
            
            var reallink = new SpiralLab.RealLink.Client("http://localhost:5000", "reallink1", "consoleapp");
          
            reallink.On("Receive",
                new[] { typeof(string), typeof(string), typeof(object) },
                (args, state) =>
                {
                    string userName = (string)args[0];
                    string message = (string)args[1];
                    object arg = args[2];
                    switch( message)
                    {
                        case "substrate":
                            var substrate = Helper.Deserialize<Substrate>(arg);
                            Console.WriteLine($"{Environment.NewLine}{DateTime.Now} Receive: {userName} {message} {substrate?.ToString()}");
                            break;
                    }
                    return Task.CompletedTask;
                });
            
            long no = 0;
            do
            {
                Console.WriteLine("Demo for RealLink c# console client - (c)SpiralLAB");
                Console.WriteLine("'1' : start");
                Console.WriteLine("'2' : reconnect");
                Console.WriteLine("'3' : send");
                Console.WriteLine("'4' : stop");
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
                        reallink.Start();
                        break;
                    case ConsoleKey.D2:
                        Console.WriteLine("Reconnecting Reallink ...");
                        reallink.Reconnect();
                        break;
                    case ConsoleKey.D3:                        
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
                            Task<bool> result = reallink.InvokeAsync<bool>("Send", "wpfapp", "substrate", substrate);
                        }
                        catch (Exception ex)
                        {
                            Console.WriteLine(ex.ToString());
                        }
                        break;
                    case ConsoleKey.D4:
                        reallink.Stop();
                        break;
                }
            } while (true);
            reallink?.Dispose();
        }
    }
}
