using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;


namespace GameServer
{
	public class Client
	{
		public int connectionID;
		public string ip;
		public TcpClient socket;

		public NetworkStream myStream;
		byte[] readBuff;

		public string username;
		public int lobbyID;
		public bool isReady;
		public bool hasRegistered = false;
		public int playerPos = 0;
		public int checkpointsCollected = 0;


		IPEndPoint ipEndpoint;
		public void Start(){
			socket.SendBufferSize = 4096;
			socket.ReceiveBufferSize = 4096;
			myStream = socket.GetStream();
			readBuff = new byte[4096];
			myStream.BeginRead(readBuff, 0, socket.ReceiveBufferSize, OnReceiveData, null);
			ServerUDP.listener.BeginReceive(new AsyncCallback(OnReceiveUDPData), ServerUDP.listener);
		}

		void OnReceiveData(IAsyncResult result){
			try{
				
				int readbytes = myStream.EndRead(result);
				if (readbytes <= 0){
					//client is not connected to the server anymore
					CloseSocket();
					return;
				}

				byte[] newBytes = new byte[readbytes];
				Buffer.BlockCopy(readBuff, 0, newBytes, 0, readbytes);
				myStream.BeginRead(readBuff, 0, 4096, OnReceiveData, null);
				//string output = System.Text.Encoding.ASCII.GetString(newBytes, 0, readbytes);
				//Text.WriteLog(output + "------" + readbytes.ToString());

				ServerHandleData.HandleData(connectionID, newBytes);
			}
			catch (Exception e)
			{
				Text.WriteError("Error Code: " + e);
				CloseSocket();
			}
		}

		void OnReceiveUDPData(IAsyncResult result) {
			Text.WriteDebug("Recieved UDP Data");
			byte[] message = ServerUDP.listener.EndReceive(result, ref ipEndpoint);
			ServerHandleData.HandleUDPData(message);

			ServerUDP.listener.BeginReceive(new AsyncCallback(OnReceiveUDPData), ServerUDP.listener);
		}

		public void CloseSocket(){
			Text.WriteLog("Connection from " + ip + " has been terminated");

			for (int i = 0; i < ServerTCP._lobbys[lobbyID]._clients.Length; i++) {
				if (ServerTCP._lobbys[lobbyID]._clients[i] == this) {
					ServerTCP._lobbys[lobbyID]._clients[i] = null;
				}
			}

			ServerTCP._lobbys[lobbyID].PlayerDisconnected(connectionID);

			try {
				socket.Close();
				socket = null;
			} catch (Exception ex) {
				socket = null;
				Text.WriteError(ex.ToString());
			}
		}

	}
}


//number messages
//limit messages
