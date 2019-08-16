#include <iostream>
#include <thread>
#include <iostream>
#include <sstream>
#include <string>

#include "NetworkManager.hpp"
using namespace std;
NetworkManager::NetworkManager(Queue<vector<std::string>> &q): queue(q){
}

void NetworkManager::ConnectTCP(){
    status = tcpSocket.connect(ipAddress, portNumber);
    if(status != sf::Socket::Done)
        isConnected = false;
    else{
        isConnected = true;
        cout<<"connected to the server!" <<endl;
    }
}

void NetworkManager::UDPSend(std::string messageToSend){
    if(udpSocket.send(messageToSend.c_str(), messageToSend.size(), ipAddress, udpPort)){
        cout<<"an error occured during send UDP data"<<endl;
    }
    cout<< "successfully sent UDP message" << endl;
}

void NetworkManager::TCPSend(std::string messageToSend){
    if(tcpSocket.send(messageToSend.c_str(), messageToSend.length()) != tcpSocket.Done){
        cout<<"an error occured during send TCP data"<<endl;
    }
    //cout << "Successully sent message --- " + messageToSend << endl;
}

void NetworkManager::TCPRecieve(){
    // loop
    while(true){
        try {
            memset(buffer, 0, sizeof(buffer));
            if(tcpSocket.receive(buffer, 4096, recieved) != sf::Socket::Done){
                cout << "an error occured during receiving data" << endl;
            }
            //std::string output(buffer);
            HandleData(buffer);
            //cout << output << endl;
        } catch (exception e) {
            cout<< e.what() << endl;
        }
    }
}

void NetworkManager::PlayerMovement(int xPos, int yPos, int angle){ //connectionID, packetIdentifier, packageID
    std::string dataToSend = "CPMove/" + std::to_string(xPos) + "/" + std::to_string(yPos) + "/" + std::to_string(angle) + "/";
    //UDPSend(dataToSend);
    //udpPackageID++;
    
    cout<< "Sending data to server: ----" +dataToSend <<endl;
    TCPSend(dataToSend);
}

void NetworkManager::ReadyPlayer(){
    std::string dataToSend = "CPlayerReady/";
    TCPSend(dataToSend);
}

void NetworkManager::AccountData(std::string Username, std::string Password){
    std::string dataToSend = "CCreateAccount/" + Username + "/" + Password;
    TCPSend(dataToSend);
}

void NetworkManager::LoginRequest(std::string Username, std::string Password){
    std::string dataToSend = "CLoginRequest/" + Username + "/" + Password;
    TCPSend(dataToSend);
}

void NetworkManager::RegisterWithServer(){
    std::string dataToSend = "CRegisterPlayer/" + general.playerUsername;
    cout << "Registered with server!" << endl;
    TCPSend(dataToSend);
}

void NetworkManager::CheckpointCollected(){
    std::string dataToSend = "CCheckpointCollected/";
    TCPSend(dataToSend);
}

void NetworkManager::GetConstantClass(General &General, SceneManager &SM){
    general = General;
    sm = SM;
}

std::vector<std::string> NetworkManager::Split(std::string strToSplit, char delimeter){
    std::stringstream ss(strToSplit);
    std::string item;
    std::vector<std::string> splitStrings;
    
    while (std::getline(ss, item, delimeter)) {
        splitStrings.push_back(item);
    }
    return splitStrings;
}

void NetworkManager::HandleData(char data[]){
    try {
        //convert char[] to string
        string content(data);
        //split the string
        vector<std::string> splitData =  Split(content, '/');
        //case statement that runs the method depending on the packet identifier
        string packetIdentifier = splitData[0];
        cout << packetIdentifier << endl;
        
        if(packetIdentifier == "SWelcomeMessage"){
            WelcomeMessage(splitData);
        }else if(packetIdentifier == "SInGame"){
            PlayerInGame(splitData);
        }else if(packetIdentifier == "SPlayerData"){
            PlayerData(splitData);
        }else if(packetIdentifier == "SPlayerDisconnected"){
            PlayerDisconnected(splitData);
        }else if(packetIdentifier == "SPlayerMovement"){
            PlayerMove(splitData);
        }else if(packetIdentifier == "SLobbyUsernames"){
            LobbyUsernames(splitData);
        }else if(packetIdentifier == "SUpdateLobbyTimer"){
            TimerCounter(splitData);
        }else if(packetIdentifier == "SUpdatePlayerPositions"){
            UpdatePlayerPositions(splitData);
        }else if(packetIdentifier == "SWaitingForPlayers"){
            WaitingForPlayers(splitData);
        }else if(packetIdentifier == "SPlayerReady"){
            PlayerIsReady(splitData);
        }else if(packetIdentifier == "SStartGame"){
            StartGame(splitData);
        }else if(packetIdentifier == "SAccountCreated"){
            AccountCreated(splitData);
        }else if(packetIdentifier == "SAccountExists"){
            AccountExists(splitData);
        }else if(packetIdentifier == "SError"){
            ErrorRecieved(splitData);
        }else if(packetIdentifier == "SLogin"){
            Login(splitData);
        }else if (packetIdentifier == "SPlayerStartPos"){
            PlayerStartPositions(splitData);
        }
    } catch (exception e) {
        cout<< "Exception Caught!" << endl;
        cout<< e.what() << endl;
    }
}

void NetworkManager::WelcomeMessage(vector<std::string> data){
    cout << data[1] << endl;
    general.isInLobby = true;
    data.clear();
}

void NetworkManager::PlayerInGame(vector<std::string> data){
    general.connectionID = stoi(data[1]);
    cout << "ConnectionID: " + data[1]<< endl;
    data.clear();
}

void NetworkManager::PlayerData(vector<std::string> data){
    string packetIdentifier = data[0];
    int connectionID = stoi(data[1]);
    string carColour = data[2];
    string username = data[3];
    
    for (int i = 0; i < playerList.size(); i++) { //playerlist is a vector array located within this class
        if(i == connectionID){
            return; //this stops the double spawning
        }
    }
    
    Client temp;
    //check to see if you are the controlling player
    if(connectionID == general.connectionID){
        //load this player as the controller
    }else{
        //load this through the client class
        temp.connectionID = connectionID;
        temp.username = username;
        
        temp.SetupGraphics(carColour);
        //add the temp player into the playerList array
        playerList.push_back(temp);
        
    }
}

void NetworkManager::PlayerDisconnected(vector<std::string> data){
    int clientID = stoi(data[1]);
    for (int i = 0; i < playerList.size(); i++) {
        if(i == clientID){
            playerList[i].Disconnected();
            playerList.erase(playerList.begin() + i);
        }
    }
    data.clear();
}

void NetworkManager::PlayerMove(vector<std::string> data){
    int connectionID = stoi(data[1]);
    int XPos = stoi(data[2]);
    int YPos = stoi(data[3]);
    int RotAngle = stoi(data[4]);
    
    for (int i = 0; i < playerList.size(); i++) {
        if(playerList[i].connectionID == connectionID){
            playerList[i].posX = XPos;
            playerList[i].posY = YPos;
            playerList[i].rotationAngle = RotAngle;
            playerList[i].Update();
            return;
        }
    }
}

void NetworkManager::LobbyUsernames(vector<std::string> data){
    string packetIdentifier = data[0];
    for (int i = 1; i < playerList.size(); i++) {
        playerList[i].username = data[i];
    }
    cout<< "Updating lobby usernames" << endl;
    queue.push(data);
    
}

void NetworkManager::PlayerIsReady(vector<std::string> data){
    cout<< "updating player ready" << endl;
    queue.push(data);
}

void NetworkManager::TimerCounter(vector<std::string> data){
    //cout<<"Timer Counter pushed to queue"<< endl;
    queue.push(data);
}

void NetworkManager::WaitingForPlayers(vector<std::string> data){
    cout << "Waiting for players" << endl;
    queue.push(data);
}

void NetworkManager::UpdatePlayerPositions(vector<std::string> data){
    
}

void NetworkManager::StartGame(vector<std::string> data){
    gameHasStarted = true;
    cout<< "starting game" << endl;
}

void NetworkManager::AccountCreated(vector<std::string> data){
    general.sentPlayerDetails = false;
    queue.push(data);
}

void NetworkManager::AccountExists(vector<std::string> data){
    general.sentPlayerDetails = false;
    queue.push(data);
}

void NetworkManager::ErrorRecieved(vector<std::string> data){
    general.sentPlayerDetails = false;
    queue.push(data);
}

void NetworkManager::Login(vector<std::string> data){
    general.sentPlayerDetails = false;
    isLoggedIn = true;
}

void NetworkManager::PlayerStartPositions(vector<std::string> data){
    for (int i = 1; i < 7;i++) {
        if(playerList[i-1].username == data[i]){
            playerList[i-1].posX = sm.spawnPoints[i-1].front();
            playerList[i-1].posY = sm.spawnPoints[i-1].back();
        }
    }
    
    
}
