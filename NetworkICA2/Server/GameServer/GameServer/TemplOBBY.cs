using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace GameServer {
	public class TemplOBBY {
		public static Client[] _clients;
		public int playersConnected = 0;
		public bool lobbyHasBeenStarted = false;
		bool gameHasStarted = false;

		static int lobbyID;
		int countdownTimer = 5;
		//int numberOfReady = 0;

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

					UpdatePlayerUsernames();

					return;
				}
			}
		}

		public void Update() {

			while (true) {
				for (int i = 0; i < _clients.Length; i++) {
					playersConnected = 0;
					if (_clients[i] != null) {
						playersConnected++;
					}
				}

				if (playersConnected >= 2) {
					gameHasStarted = true;
					StartGame();
				} else if (!gameHasStarted) {
					StartTimer();
				}
			}


			//} else if(numberOfPlayers == 1){
			//	string dataToSend = ServerPackets.SWaitingForPlayers.ToString() + General.dataSplitter;
			//	byte[] data = Encoding.ASCII.GetBytes(dataToSend);
			//	ServerTCP.SendToAllInLobby(this, data);
			//}
			//lobby loop if its needed



		}

		public void StartTimer() {
			for (int i = countdownTimer; i >= 0; i--) {
				Thread.Sleep(1000);

				UpdateLobbyTimer(i);
				//Text.WriteDebug(i + " on lobby: " + lobbyID);

			}
		}

		public void SendWelcomeMessage(int connectionID) {
			string dataTosend = ServerPackets.SWelcomeMessage.ToString() + General.dataSplitter + "Welcome to the game, you are connected to lobby: " + General.dataSplitter + lobbyID.ToString();
			byte[] data = Encoding.ASCII.GetBytes(dataTosend);

			for (int i = 0; i < _clients.Length; i++) {
				if (_clients[i].connectionID == connectionID) {
					ServerTCP.SendDataTo(_clients[i].connectionID, data);
					return;
				}
			}
			Text.WriteLog("Sent Data: " + dataTosend + "to: " + connectionID);
		}

		public void UpdatePlayerUsernames() {
			string dataToSend = ServerPackets.SLobbyUsernames.ToString();

			//all the players connected get usernames sent
			for (int i = 0; i < _clients.Length; i++) {
				if (_clients[i] != null) {
					if (_clients[i].socket != null) {
						dataToSend += General.dataSplitter + _clients[i].username;
					}
				}
			}
			byte[] data = Encoding.ASCII.GetBytes(dataToSend);
			SendToAllInLobby(data);
			Text.WriteLog("Updated players usernames on lobby: " + lobbyID + " with: " + dataToSend);
		}

		public void StartGame() {
			string dataToSend = ServerPackets.SStartGame.ToString();
			byte[] data = Encoding.ASCII.GetBytes(dataToSend);
			SendToAllInLobby(data);
			Text.WriteLog("Starting Game on: " + lobbyID);
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
			SendToAllInLobby(data);
		}

		public void UpdatePlayerReady(int connectionID) {
			string dataToSend = ServerPackets.SPlayerReady.ToString() + General.dataSplitter + connectionID;
			byte[] data = Encoding.ASCII.GetBytes(dataToSend);
			Text.WriteDebug(dataToSend);
			SendToAllInLobby(data);
		}

		public void PlayerMovement(int connectionID, int xPos, int yPos, int rotationAngle) {
			string dataToSend = ServerPackets.SPlayerMovement.ToString() + General.dataSplitter + connectionID + General.dataSplitter + xPos + General.dataSplitter + yPos + General.dataSplitter + rotationAngle;
			byte[] data = Encoding.ASCII.GetBytes(dataToSend);
			SendToAllInLobbyBut(connectionID, data);
		}

		public void PlayerDisconnected(int connectionID) {

			for (int i = 0; i < _clients.Length; i++) {
				if (_clients[i].connectionID == connectionID) {
					_clients[i] = null;
					return;
				}
			}

			string dataToSend = ServerPackets.SPlayerDisconnected.ToString() + General.dataSplitter + connectionID;
			byte[] data = Encoding.ASCII.GetBytes(dataToSend);
			SendToAllInLobby(data);

		}

		public void CheckPlayerPositions() {
			Client[] tempClients = new Client[6];
			tempClients = _clients;

			//sort the array 

			string dataToSend = ServerPackets.SUpdatePlayerPositions.ToString() + General.dataSplitter;
			for (int i = 0; i < tempClients.Length; i++) {
				dataToSend += tempClients[i].connectionID + General.dataSplitter;
			}

			byte[] data = Encoding.ASCII.GetBytes(dataToSend);
			SendToAllInLobby(data);
		}



		public static void SendToAllInLobby(byte[] data) {
			try {
				for (int i = 0; i < _clients.Length; i++) {
					if (_clients[i] != null) {
						if (_clients[i].socket != null) {
					ServerTCP.SendDataTo(_clients[i].connectionID, data);
							Text.WriteDebug("Sending data to: " + _clients[i].username);
						}
					}
				}
			} catch (Exception ex) {
				Text.WriteDebug(ex.ToString());
			}
		}

		public static void SendToAllInLobbyBut(int connectionID, byte[] data) {
			for (int i = 0; i < _clients.Length; i++) {
				if (connectionID != i) {
					if (_clients[i] != null) {
						if (_clients[i].socket != null) {
							ServerTCP.SendDataTo(_clients[i].connectionID, data);
						}
					}
				}
			}
		}

	}
}