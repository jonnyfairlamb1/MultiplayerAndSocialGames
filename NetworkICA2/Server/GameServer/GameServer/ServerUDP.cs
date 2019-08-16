using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace GameServer { 
	
public class ServerUDP{
		public const int listenPort = 5556;
		public static UdpClient listener = new UdpClient(listenPort);
	} 
}
