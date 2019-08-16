using System;
using Mono.Data.Sqlite;
using System.Data;

namespace GameServer {
	public class Database {

		public static SqliteConnection db_connection;

		public static void Start() {
			db_connection = new SqliteConnection("Data Source=NetworkingDatabase.sqlite;");
			db_connection.Open();
		}

		public static bool CheckAccountExists(string Username, string Password) {
			string sql = "SELECT * FROM users WHERE username='" + Username + "'";
			SqliteCommand command = new SqliteCommand(sql, db_connection);
			SqliteDataReader reader = command.ExecuteReader();

			if (reader.Read()) {
				reader.Close();
				Text.WriteDebug("User exists with username: " + Username);
				return true;
			} else {
				reader.Close();
				Text.WriteDebug("User doesnt exist");
				return false;
			}
		}

		public static void AddAccount(string Username, string Password) {
			string sql = "INSERT INTO users VALUES ('" + Username + "','" + Password + "')";
			SqliteCommand command = new SqliteCommand(sql, db_connection);
			command.ExecuteNonQuery();
			Text.WriteDebug("Adding user to the database");
		}

		public static bool AccountDetailsAreCorrect(string Username, string Password) {
			string sql = "SELECT * FROM users WHERE username='" + Username + "' AND password='" + Password + "'";
			SqliteCommand command = new SqliteCommand(sql, db_connection);
			SqliteDataReader reader = command.ExecuteReader();

			if (reader.Read()) {
				reader.Close();
				Text.WriteDebug("Login Successful");
				return true;
			} else {
				reader.Close();
				Text.WriteDebug("Incorrect Username/password");
				return false;
			}
		}
	}
}
