using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace GameServer
{
	public class ServerTCP
	{
		static TcpListener serverSocket;
		public static Client[] Client = new Client[Constants.MAX_PLAYERS];
		public static Lobby[] _lobbys = new Lobby[100];


		public static void InitializeNetwork() {
			serverSocket = new TcpListener(IPAddress.Any, 5555);
			serverSocket.Start();
			serverSocket.BeginAcceptTcpClient(OnClientConnect, null);

		}

		static void OnClientConnect(IAsyncResult result) { 
			TcpClient client = serverSocket.EndAcceptTcpClient(result);
			serverSocket.BeginAcceptTcpClient(OnClientConnect, null);

            for (int i = 0; i<Constants.MAX_PLAYERS; i++)
            {
                if (Client[i].socket == null)
                {
                    Client[i].socket = client;
                    Client[i].connectionID = i;
                    Client[i].ip = client.Client.RemoteEndPoint.ToString();
                    Client[i].Start();
					Text.WriteLog("Connection received from " + Client[i].ip + " | ConnectionID: " + Client[i].connectionID);
                    return;
                }
            }
		}

		public static void SendDataTo(int connectionID, byte[] data) {
			//maybe encrypt the data here?
			for (int i = 0; i < Client.Length;i++) {
				if (i == connectionID) {
					Client[i].myStream.BeginWrite(data, 0, data.Length, null, null);
				}
			}
		}

		public static void SendDataToAll(byte[] data) {
			for (int i = 0; i < Constants.MAX_PLAYERS; i++){
				if (Client[i].socket != null) {
					SendDataTo(Client[i].connectionID, data);
				}
			}
		}

		public static void SendDataToAllBut(int connectionID, byte[] data)
		{
			for (int i = 0; i < Constants.MAX_PLAYERS; i++)
			{
				if (connectionID != i)
					if (Client[i].socket != null)
                    	SendDataTo(Client[i].connectionID, data);
			}
		}

		public static void SendToAllInLobby(Lobby lobby, byte[] data) {
			for (int i = 0; i < lobby._clients.Length; i++) {
				if (lobby._clients[i] != null && lobby._clients[i].socket != null) {
					SendDataTo(lobby._clients[i].connectionID, data);
				}
			}
		}

		public static void SendToAllInLobbyBut(Lobby lobby, int connectionID, byte[] data) {
			for (int i = 0; i < lobby._clients.Length; i++) {
				if (lobby._clients[i] != null && lobby._clients[i].connectionID != connectionID) {
					SendDataTo(lobby._clients[i].connectionID, data);
				}
			}
		}

		public static void MatchMaker(Client client) {
			for (int i = 0; i < _lobbys.Length; i++)
			{		if (_lobbys[i].playersConnected < 6 && _lobbys[i].lobbyHasBeenStarted) { //Is there room for this player to join the lobby
						_lobbys[i].JoinLobby(client);
						return;
					} else {
						_lobbys[i].StartLobby(i);
						MatchMaker(client);
					}
				} 
		}

		public static void AccountCreated(long ConnectionID) {
			string dataToSend = ServerPackets.SAccountCreated.ToString();
			byte[] data = Encoding.ASCII.GetBytes(dataToSend);
			SendDataTo((int)ConnectionID, data);
		}

		public static void AccountExists(long ConnectionID) { 
			string dataToSend = ServerPackets.SAccountExists.ToString();
			byte[] data = Encoding.ASCII.GetBytes(dataToSend);
			SendDataTo((int)ConnectionID, data);
		}

		public static void Login(long ConnectionID) {
			string dataToSend = ServerPackets.SLogin.ToString();
			byte[] data = Encoding.ASCII.GetBytes(dataToSend);
			SendDataTo((int)ConnectionID, data);
		}

		public static void Error(long ConnectionID) {
			string dataToSend = ServerPackets.SError.ToString() + "/Incorrect Username or Password";
			byte[] data = Encoding.ASCII.GetBytes(dataToSend);
			SendDataTo((int)ConnectionID, data);
		}

	}

}