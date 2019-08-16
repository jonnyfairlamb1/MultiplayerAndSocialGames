using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
	public class Text
	{
		public static void WriteInfo(string text)
		{
			Console.ForegroundColor = ConsoleColor.Blue;
			Console.WriteLine("[INFO]" + text);
			Console.ResetColor();
		}

		public static void WriteDebug(string text)
		{
			Console.ForegroundColor = ConsoleColor.DarkCyan;
			Console.WriteLine("[Debug]" + text);
			Console.ResetColor();
		}
		public static void WriteLog(string text)
		{
			Console.ForegroundColor = ConsoleColor.White;
			Console.WriteLine("[Log]" + text);
			Console.ResetColor();
		}

		public static void WriteWarning(string text) {
			Console.ForegroundColor = ConsoleColor.DarkYellow;
			Console.WriteLine("[Warning]" + text);
			Console.ResetColor();
		}

		public static void WriteError(string text){
			Console.ForegroundColor = ConsoleColor.DarkRed;
			Console.WriteLine("[Error]" + text);
			Console.ResetColor();
		}

		public static void ClearLog() {
			Console.Clear();
		}
	}
}
