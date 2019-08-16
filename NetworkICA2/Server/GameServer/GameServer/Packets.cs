public enum ServerPackets {
	SWelcomeMessage = 1,
	SInGame,
	SPlayerData,
	SPlayerDisconnected,
	SPlayerMovement,
	SLobbyUsernames,
	SUpdateLobbyTimer,
	SPlayerReady,
	SStartGame,
	SUpdatePlayerPositions,
	SWaitingForPlayers,
	SAccountCreated,
	SAccountExists,
	SLogin,
	SError,
	SPlayerStartPos,
}

public enum ClientPackets { 
	CRegisterPlayer = 1,
	CCreateAccount,
	CLoginRequest,
	CPMove,
	CPlayerReady,
	CCheckpointCollected,

}