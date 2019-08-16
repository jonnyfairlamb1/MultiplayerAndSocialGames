using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace GameServer
{
	public class ServerHandleData
	{
		static char[] delimiters = {'/'};
		static int previousUDPPackageID = 0;
		//static string[] movementData;

		public static void HandleData(long connectionID, byte[] data) {
			string content = System.Text.Encoding.ASCII.GetString(data, 0, data.Length);
			//Split the packet identifier out
			string[] newData = content.Split(delimiters);
			string packetIdentifier = newData[0];
			//check what the packet identifier reads

			if (packetIdentifier == "CRegisterPlayer") {
				PACKET_PLAYERREGISTER(connectionID, newData);
			} else if (packetIdentifier == "CPMove") {
				PACKET_PLAYERMOVEMENT(connectionID, newData);
			} else if (packetIdentifier == "CPlayerReady") {
				PACKET_PLAYERREADY(connectionID, newData);
			} else if (packetIdentifier == "CCheckpointCollected") {
				PACKET_CHECKPOINTCOLLECTED(connectionID, newData);
			} else if (packetIdentifier == "CCreateAccount") {
				PACKET_PLAYERACCOUNTDATA(connectionID, newData);
			} else if (packetIdentifier == "CLoginRequest") {
				PACKET_LOGINREQUEST(connectionID, newData);
			}
		}

		public static void HandleUDPData(byte[] data) {
			string content = System.Text.Encoding.ASCII.GetString(data, 0, data.Length);
			//Split the packet identifier out
			string[] newData = content.Split(delimiters);
			int connectionID = Int32.Parse(newData[0]);
			string packetIdentifier = newData[1];

			int packageNumber = Int32.Parse(newData[2]);
			if (packageNumber > previousUDPPackageID) {
				return;
			} else { 
				//check what the packet identifier reads

				if (packetIdentifier == "CPMove") {
	                PACKET_PLAYERMOVEMENT(connectionID, newData);
				} 
			}
			previousUDPPackageID = packageNumber;

		}

		static void PACKET_PLAYERACCOUNTDATA(long connectionID, string[] data) {
			string playerUsername = data[1];
			string playerPassword = data[2];

			//see if the account is in use or not

			if (!Database.CheckAccountExists(playerUsername, playerPassword)) {
				Database.AddAccount(playerUsername, playerPassword);
				ServerTCP.AccountCreated(connectionID);
			} else {
				ServerTCP.AccountExists(connectionID);
			}

			Text.WriteLog("Recieved account details");
		}

		static void PACKET_LOGINREQUEST(long ConnectionID, string[] Data) { 
			string playerUsername = Data[1];
			string playerPassword = Data[2];

			if (Database.AccountDetailsAreCorrect(playerUsername, playerPassword)) {
				for (int i = 0; i < ServerTCP.Client.Length; i++) {
					if (ServerTCP.Client[i] != null) {
						if (ServerTCP.Client[i].connectionID == ConnectionID) {
							ServerTCP.Client[i].username = playerUsername;
							ServerTCP.Login(ConnectionID);
							return;
						}
					}
				}
			} else {
				//wrong username/password
				ServerTCP.Error(ConnectionID);
			}
		}

		static void PACKET_PLAYERREGISTER(long connectionID, string[] data) {
			string username = data[1];
			for (int i = 0; i < ServerTCP.Client.Length; i++) {
				if (i == connectionID) {
					ServerTCP.Client[i].hasRegistered = true;
					ServerTCP.MatchMaker(ServerTCP.Client[i]);
				}
			}
		}

		static void PACKET_PLAYERMOVEMENT(long connectionID, string[] data) {
			int xPos = Int32.Parse(data[1]);
			int yPos = Int32.Parse(data[2]);
			int rotationAngle = Int32.Parse(data[3]);

			Text.WriteLog("Recieved data from: " + connectionID.ToString() + "with values: " + xPos + "-----" + yPos + "-----" + rotationAngle);

			//find the lobby this player is in

			for (int i = 0; i < Constants.MAX_PLAYERS; i++) {
				if (ServerTCP.Client[i] != null && ServerTCP.Client[i].connectionID == connectionID) {
					ServerTCP._lobbys[ServerTCP.Client[i].lobbyID].UpdatePlayerMovement(xPos, yPos, rotationAngle, (int)connectionID);
					return;
				}
			}
		}

		static void PACKET_PLAYERREADY(long connectionID, string[] data) {
			for (int i = 0; i < ServerTCP.Client.Length; i++) {
				if (ServerTCP.Client[i].connectionID == (int)connectionID) {
					ServerTCP.Client[i].isReady = true;
					//send back to the players
					ServerTCP._lobbys[ServerTCP.Client[i].lobbyID].UpdatePlayerReady(ServerTCP.Client[i]);

					return;
				}
			}
		}

		static void PACKET_CHECKPOINTCOLLECTED(long connectionID, string[] data) {
			for (int i = 0; i < ServerTCP.Client.Length; i++) {
				if (ServerTCP.Client[i].connectionID == connectionID) {
					ServerTCP.Client[i].checkpointsCollected++;
					//ServerTCP._lobbys[ServerTCP.Client[i].lobbyID].CheckPlayerPositions();
				}
			}

		}
	}
}
