//
//  NetworkManager.hpp
//  t01
//
//  Created by FAIRLAMB, JONNY on 08/10/2018.
//  Copyright © 2018 Cordry, Julien. All rights reserved.
//

#pragma once
#ifndef NetworkManager_hpp
#define NetworkManager_hpp

#include <stdio.h>
#include <iostream>
#include <thread>
#include "Client.hpp"
#include "General.hpp"
#include <SFML/Network.hpp>
#include "Queue.hpp"
#include "SceneManager.hpp"
#endif /* NetworkManager_hpp */

using namespace std;

class NetworkManager{
    
public:
    NetworkManager(Queue<vector<string>> &q);
    void ConnectTCP();
    void UDPBind();
    void UDPSend(std::string messageToSend);
    void TCPSend(std::string messageToSend);
    void TCPRecieve();
    void GetConstantClass(General &General, SceneManager &SM);
    
    void AccountData(std::string Username, std::string Password);
    void LoginRequest(std::string Username, std::string Password);
    void RegisterWithServer();
    void PlayerMovement(int xPos, int yPos, int angle);
    void ReadyPlayer();
    void CheckpointCollected();
    
    char buffer[4096];
    std::size_t recieved;
    
    bool testRecieve = false;
    bool errorHasBeenSent = false;
    bool isConnected = false;
    bool isLoggedIn = false;
    
    sf::Socket::Status status;
    std::vector<Client> playerList;
    bool gameHasStarted = false;
private:
    std::string ipAddress = "152.105.5.147";
    int portNumber = 5555;
    int udpPackageID = 0;
    unsigned short udpPort = 5556;
    
    sf::TcpSocket tcpSocket;
    sf::UdpSocket udpSocket;
    
    Queue<vector<std::string>> &queue;
    General general;
    SceneManager sm;
    void HandleData(char data[]);
    //0 = packet identifier, this is only used in the handle data
    void WelcomeMessage(vector<std::string> data); //1 = Welcome to the world message from server
    void PlayerInGame(vector<std::string> data); //1 = this players connectionID
    void PlayerData(vector<std::string> data); //1 = connectionID, 2 = colour of the car that player has chosen, 3 = players username
    void PlayerDisconnected(vector<std::string> data); //1 = the connectionID of the player that has disconnected
    void PlayerMove(vector<std::string> data);//1 = connectionID, 2 = X position, 3 = Y position, 4 = rotation Angle
    void LobbyUsernames(vector<std::string> data); //1 = player1, 2 = player2 etc
    void PlayerIsReady(vector<std::string> data); //1 = connectionID of ready player
    void TimerCounter(vector<std::string> data); //1 = timer on the server
    void UpdatePlayerPositions(vector<std::string> data); //1 = 1st place, 2 = 2nd place etc
    void WaitingForPlayers(vector<std::string> data); //Waiting for players in the lobby so dont do anything
    void StartGame(vector<std::string> data); //just the packet identifier
    void AccountCreated(vector<std::string> data);
    void AccountExists(vector<std::string> data);
    void ErrorRecieved(vector<std::string> data);
    void Login(vector<std::string> data);
    void PlayerStartPositions(vector<std::string> data);
    
    std::vector<std::string> Split(std::string strToSplit, char delimeter);
    
    
};

