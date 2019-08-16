using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace GameServer {
	public class Lobby {
		public Client[] _clients;
		public int playersConnected = 0;

		int playersReady = 0;
		public bool lobbyHasBeenStarted = false;
		public bool gameHasStarted = false;

		static int lobbyID;
		int countdownTimer = 60;

		public void StartLobby(int LobbyID) {
			_clients = new Client[6];
			lobbyID = LobbyID;
			lobbyHasBeenStarted = true;
		}

		public void JoinLobby(Client client) {
			for (int i = 0; i < _clients.Length; i++) {
				if (_clients[i] == null) {
					_clients[i] = client;
					_clients[i].lobbyID = lobbyID;
					Text.WriteLog("User: " + _clients[i].username + " Connected to Lobby number: " + lobbyID);
					SendWelcomeMessage(_clients[i].connectionID);
					PlayerInGame(_clients[i].connectionID);
					//update usernames
					playersConnected++;
					UpdatePlayerUsernames();
					UpdateAllPlayerReady();
					return;
				}
			}
		}

		public void Update() {
			while (true) {
				 if (playersConnected >= 1 && !gameHasStarted) {
					StartTimer();
					return;
				}
			}
		}

		public void StartTimer() {
			for (int i = countdownTimer; i >= 0; i--) {
				Thread.Sleep(1000);

				UpdateLobbyTimer(i);

				if (i <= 0 && !gameHasStarted || playersReady == playersConnected && !gameHasStarted) {
					gameHasStarted = true;
					StartGame();
					Thread.Sleep(Timeout.Infinite);
				}
			}
		}

		public void SendWelcomeMessage(int connectionID) {
			string dataTosend = ServerPackets.SWelcomeMessage.ToString() + General.dataSplitter + "Welcome to the game, you are connected to lobby: " + General.dataSplitter + lobbyID.ToString();
			byte[] data = Encoding.ASCII.GetBytes(dataTosend);

			for (int i = 0; i<_clients.Length; i++) {
				if (_clients[i].connectionID == connectionID) {
					ServerTCP.SendDataTo(_clients[i].connectionID, data);
					return;
				}
			}
		}

		public void UpdatePlayerUsernames() {
			string dataToSend = ServerPackets.SLobbyUsernames.ToString();
			//all the players connected get usernames sent
			for (int i = 0; i<_clients.Length; i++) {
				if (_clients[i] != null) {
					if (_clients[i].socket != null) {
						dataToSend += General.dataSplitter + _clients[i].username;
					}
				}
			}

			Text.WriteInfo(dataToSend);
			byte[] data = Encoding.ASCII.GetBytes(dataToSend);
			ServerTCP.SendToAllInLobby(this, data);
		}

		public void UpdatePlayerReady(Client client) { 
			string dataToSend = ServerPackets.SPlayerReady.ToString() + General.dataSplitter + client.username;
			byte[] data = Encoding.ASCII.GetBytes(dataToSend);
			playersReady++;
			ServerTCP.SendToAllInLobby(this, data);
		}

		public void UpdateAllPlayerReady() {
			for (int i = 0; i < _clients.Length; i++) {
				if (_clients[i] != null) { 
					if (_clients[i].isReady) {
                   		UpdatePlayerReady(_clients[i]);
					}
				}
			}
		}

		public void StartGame() {
			string dataToSend = ServerPackets.SStartGame.ToString();
			byte[] data = Encoding.ASCII.GetBytes(dataToSend);
			ServerTCP.SendToAllInLobby(this, data);
			Text.WriteLog("Starting Game on: " + lobbyID);
			PlayerData();
			SetStartPoints();
		}

		public void PlayerInGame(int connectionID) {
			string dataToSend = ServerPackets.SInGame.ToString() + General.dataSplitter + connectionID.ToString();
			byte[] data = Encoding.ASCII.GetBytes(dataToSend);

			for (int i = 0; i < _clients.Length; i++) {
				if (_clients[i].connectionID == connectionID) {
					ServerTCP.SendDataTo(_clients[i].connectionID, data);
					return;
				}
			}
		}

		public void UpdateLobbyTimer(int time) {
			string dataToSend = ServerPackets.SUpdateLobbyTimer.ToString() + General.dataSplitter + time.ToString();
			byte[] data = Encoding.ASCII.GetBytes(dataToSend);
			ServerTCP.SendToAllInLobby(this, data);
		}

		public void PlayerDisconnected(int connectionID) {
			playersConnected--;
			playersReady--;
            UpdatePlayerUsernames();
			string dataToSend = ServerPackets.SPlayerDisconnected.ToString() + General.dataSplitter + connectionID;
			byte[] data = Encoding.ASCII.GetBytes(dataToSend);
			ServerTCP.SendToAllInLobby(this, data);
		}

		public void PlayerData() {
			for (int i = 0; i < _clients.Length; i++) {
				if (_clients[i] != null) {
					string dataToSend = ServerPackets.SPlayerData.ToString()+ General.dataSplitter + _clients[i].connectionID + General.dataSplitter + "" + General.dataSplitter + _clients[i].username;
					byte[] data = Encoding.ASCII.GetBytes(dataToSend);
					ServerTCP.SendToAllInLobby(this, data);
				}
			}
		}

		public void UpdatePlayerMovement(int Xpos, int Ypos, int RotAngle, int connectionID) {
			string dataToSend = ServerPackets.SPlayerMovement.ToString() + General.dataSplitter + connectionID.ToString() + General.dataSplitter + Xpos + General.dataSplitter + Ypos + General.dataSplitter + RotAngle;

			Text.WriteLog(dataToSend);

			byte[] data = Encoding.ASCII.GetBytes(dataToSend);
			ServerTCP.SendToAllInLobbyBut(this, connectionID, data);
		}

		public void SetStartPoints() {
			string dataToSend = ServerPackets.SPlayerStartPos.ToString();

			for (int i = 0; i < _clients.Length;i++) {
				if (_clients[i] != null) {
					dataToSend += General.dataSplitter + _clients[i].username + General.dataSplitter;
				}
			}
			byte[] data = Encoding.ASCII.GetBytes(dataToSend);
			ServerTCP.SendToAllInLobby(this, data);
		}

	}
}
