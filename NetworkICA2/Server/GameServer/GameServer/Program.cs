using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace GameServer {

	public class Program
	{
		static string command;
		static Thread threadConsole;

		static void Main(String[] args)
		{
			threadConsole = new Thread(new ThreadStart(ConsoleThread));
			threadConsole.Start();
			General.InitializeServer();

			for (int i = 0; i < ServerTCP._lobbys.Length; i++) {
				ServerTCP._lobbys[i].Update();
			}
		}

		static void ConsoleThread() {
			string line; string[] parts;

			while (true)
			{
				line = Console.ReadLine();
				parts = line.Split(' ');

				command = Console.ReadLine();
				Text.WriteDebug(command);
				if (command == "Close") {
					CloseServer();
				}
			}
		}

		static void CloseServer() {
			Database.db_connection.Close();
			//close all client connections
			for (int i = 0; i < ServerTCP.Client.Length; i++) {
				ServerTCP.Client[i].CloseSocket();
			}
			Environment.Exit(0);
		}
	}
}
