using System;
namespace GameServer
{
	public class General
	{

		public static string dataSplitter = "/";


		public static int GetTickCount() {
			return Environment.TickCount;
		}

		public static void InitializeServer()
		{
			int startTime = 0; int endTime = 0;
			startTime = GetTickCount();
			Text.WriteDebug("Initializing Server...");
			//Intializing all game data arrays
			Text.WriteDebug("Initializing Game Arrays...");
			for (int i = 0; i < Constants.MAX_PLAYERS; i++)
			{
				ServerTCP.Client[i] = new Client();
			}

			Text.WriteDebug("Initializing Lobbys");
			for (int i = 0; i < Constants.MAX_LOBBYS; i++)
			{
				ServerTCP._lobbys[i] = new Lobby();
			}

			ServerTCP._lobbys[0].StartLobby(0);

			//Start the Networking
			Text.WriteDebug("Initializing Network...");
			ServerTCP.InitializeNetwork();
			//ServerUDP.UpdateUDP();

			Text.WriteDebug("Initializing Database...");
			Database.Start();
			endTime = GetTickCount();
			Text.WriteInfo("Initialization complete. Server loaded in " + (endTime - startTime) + " ms.");
		}		
	}
}
