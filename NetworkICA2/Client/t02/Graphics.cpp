//
//  Graphics.cpp
//  t01
//
//  Created by FAIRLAMB, JONNY on 08/10/2018.
//  Copyright © 2018 Cordry, Julien. All rights reserved.
//

#include "Graphics.hpp"
#include <iostream>
#include "SceneManager.hpp"
#include <SFML/Graphics.hpp>

using namespace std;

Graphics::Graphics(Queue<vector<std::string>> &q): queue(q){
    
}

void Graphics::GetClasses(General &General){
    general = General;
}

void Graphics::BuildUsernameInputScene(){
    LoadText("UsernameInput");
}

void Graphics::BuildMap1Scene(){
    LoadBackground();
    LoadPlayer();
    LoadText("Racetrack");
    
    if(!checkpointTex.loadFromFile(DataPath + "arrow_yellow.png")){
        std::cout <<"checkpoint Texture didnt load!" <<endl;
    }
    
    for (int i = 0; i <= 8; i++) {
        checkpointSprite[i].setTexture(checkpointTex);
        checkpointSprite[i].setOrigin(41.25f, 24.75f);
        checkpointSprite[i].setScale(.6f, .6f);
    }
    
    checkpointSprite[0].setPosition(1650, 93);
    checkpointSprite[0].setRotation(90);
    checkpointSprite[1].setPosition(1772, 900);
    checkpointSprite[1].setRotation(180);
    checkpointSprite[2].setPosition(350, 1010);
    checkpointSprite[2].setRotation(270);
    checkpointSprite[3].setPosition(970, 450);
    checkpointSprite[4].setPosition(600, 93);
    checkpointSprite[4].setRotation(90);
    
    rect.setSize(sf::Vector2f(30, 110));
    rect.setPosition(950, 65);
    rect.setFillColor(sf::Color(0,0,0,0));
    
    
    std::cout <<"Built the map" << endl;
}

void Graphics::BuildMainMenuScene(){
    LoadText("MainMenu");
}

void Graphics::BuildLobbyScene(){
    if(!carSelectionTex[0].loadFromFile(DataPath + "car_black_1.png")){
        std::cout <<"black car Texture didnt load!" <<endl;
    }
    cout<< "lobby black car loaded" << endl;
    if(!carSelectionTex[1].loadFromFile(DataPath + "car_blue_1.png")){
        std::cout <<"blue car Texture didnt load!" <<endl;
    }
    cout<< "lobby blue car loaded" << endl;
    if(!carSelectionTex[2].loadFromFile(DataPath + "car_green_1.png")){
        std::cout <<"green car Texture didnt load!" <<endl;
    }
    cout<< "lobby green car loaded" << endl;
    if(!carSelectionTex[3].loadFromFile(DataPath + "car_red_1.png")){
        std::cout <<"red car Texture didnt load!" <<endl;
    }
    cout<< "lobby red car loaded" << endl;
    if(!carSelectionTex[4].loadFromFile(DataPath + "car_yellow_1.png")){
        std::cout <<"yellow car Texture didnt load!" <<endl;
    }
    cout<< "lobby yellow car loaded" << endl;
    if(!arrow.loadFromFile(DataPath + "Arrow.png")){
        std::cout << "arrow texture didnt load!" << endl;
    }
    
    
    
    carSelectionSprite.setTexture(carSelectionTex[selectedCar]);
    carSelectionSprite.setPosition(180, 255);
    carSelectionSprite.setScale(3.5f, 3.5f);
    
    arrowLeft.setTexture(arrow);
    arrowLeft.setRotation(180);
    arrowLeft.setPosition(200, 450);
    arrowLeft.setOrigin(400, 162);
    arrowRight.setTexture(arrow);
    arrowRight.setPosition(300, 426);
    
    arrowLeft.setScale(0.15, 0.15);
    arrowRight.setScale(0.15, 0.15);
    cout<< "lobby positions set" << endl;
    LoadText("Lobby");
    
}

void Graphics::HandleQueue(std::vector<Client> PlayerList){
    vector<std::string> splitData = queue.pop();
    string packetIdentifier = splitData[0];
    
    if(packetIdentifier == "SLobbyUsernames"){
        LobbyUsernames(splitData);
    }else if(packetIdentifier == "SUpdateLobbyTimer"){
        UpdateTimerGraphics(splitData);
    }else if(packetIdentifier == "SWaitingForPlayers"){
        UpdateWaitingforPlayersText(splitData);
    }else if(packetIdentifier == "SPlayerReady"){
        PlayerReady(splitData, PlayerList);
    }else if(packetIdentifier == "SAccountCreated"){
        AccountCreated();
    }else if(packetIdentifier == "SAccountExists"){
        AccountExists();
    }else if(packetIdentifier == "SError"){
        Error(splitData);
    }
}

void Graphics::LobbyUsernames(vector<std::string> data){
    for (int i = 1; i <= 7; i++) { //the last 6 objects are going to be usernames, the first is packet identifier
        if(i < data.size()){
            playerUsernames[i-1].setString(data[i]);
        }else{
            playerUsernames[i-1].setString("Waiting For Players...");
            readyButton.setString("Ready!");
        }
    }
}

void Graphics::UpdateTimerGraphics(vector<std::string> data){
    countdownTimerText.setString(data[1]);
}

void Graphics::UpdateWaitingforPlayersText(vector<std::string> data){
    
}

void Graphics::PlayerReady(vector<std::string> data, std::vector<Client> PlayerList){
    //loop through username text
    //if username == data
    //set colour to green
    
    for (int i = 0; i < 6; i++) {
        if(playerUsernames[i].getString() == data[1]){
            playerUsernames[i].setFillColor(sf::Color::Green);
        }
    }
}

void Graphics::AccountExists(){
    errorText.setString("Account Exists. Please try a different username");
    general.playerUsername = "";
    general.playerPassword = "";
    general.visualPlayerPassword = "";
    passWordText.setString("Input Password");
    playerUsernameText.setString("Input Username");
}

void Graphics::AccountCreated(){
    errorText.setString("Account Created! Please login");
    general.playerUsername = "";
    general.playerPassword = "";
    general.visualPlayerPassword = "";
    passWordText.setString("Input Password");
    playerUsernameText.setString("Input Username");
}

void Graphics::Error(vector<std::string> data){
    errorText.setString("Incorrect Username Or Password. Please Try Again");
    general.playerUsername = "";
    general.playerPassword = "";
    general.visualPlayerPassword = "";
    passWordText.setString("Input Password");
    playerUsernameText.setString("Input Username");
}

void Graphics::LoadBackground(){
    if(!backgroundColliderTex.loadFromFile(DataPath + "Background_Collider.png")){
        std::cout <<"Background Texture didnt load!" <<endl;
    }
    backgroundColliderSprite.setTexture(backgroundColliderTex);
    
    if(!raceTrackTex.loadFromFile(DataPath + "Racetrack.png")){
        std::cout <<"Background Texture didnt load!" <<endl;
    }
    raceTrackSprite.setTexture(raceTrackTex);
    
    std::cout << "Background loaded" << endl;
}

void Graphics::LoadPlayer(){
    if(!playerTex.loadFromFile(DataPath + "car_black_1.png")){
        std::cout <<"Background Texture didnt load!" <<endl;
    }
    playerSprite.setTexture(playerTex);
    std::cout << "Player loaded" << endl;
    
    playerSprite.setOrigin(55, 18);
    playerSprite.setPosition(200, 200);
}

void Graphics::LoadText(std::string SceneName){
    if(!font.loadFromFile(DataPath + "arial.ttf")){
        cout<<"could not load font file"<<endl;
    }
    if(SceneName == "UsernameInput"){
        playerUsernameText.setFont(font);
        playerUsernameText.setString("Input Username");
        playerUsernameText.setCharacterSize(24);
        playerUsernameText.setFillColor(sf::Color::Red);
        playerUsernameText.setPosition(1920/2, 540);
        
        passWordText.setFont(font);
        passWordText.setString("Input Password");
        passWordText.setCharacterSize(24);
        passWordText.setFillColor(sf::Color::Red);
        passWordText.setPosition(1920/2, 600);
        
        confirmText.setFont(font);
        confirmText.setString("Confirm");
        confirmText.setCharacterSize(24);
        confirmText.setFillColor(sf::Color::Red);
        confirmText.setPosition(1600, 800);
        
        backText.setFont(font);
        backText.setString("Back");
        backText.setCharacterSize(24);
        backText.setFillColor(sf::Color::Red);
        backText.setPosition(300, 800);
        
        errorText.setFont(font);
        errorText.setString("");
        errorText.setCharacterSize(24);
        errorText.setFillColor(sf::Color::Red);
        errorText.setPosition(1920/2, 200);
        
        createAccountText.setFont(font);
        createAccountText.setString("Create Account");
        createAccountText.setCharacterSize(24);
        createAccountText.setFillColor(sf::Color::Red);
        createAccountText.setPosition(1920/2, 800);
        
        cout<< "Login Screen Loaded" << endl;
    }else if(SceneName == "MainMenu"){
        joinGameText.setFont(font);
        joinGameText.setString("Join Game");
        joinGameText.setCharacterSize(24);
        joinGameText.setFillColor(sf::Color::Red);
        joinGameText.setPosition(960, 450);
        
        quitGameText.setFont(font);
        quitGameText.setString("Quit Game");
        quitGameText.setCharacterSize(24);
        quitGameText.setFillColor(sf::Color::Red);
        quitGameText.setPosition(960, 545);
        cout << "Main menu text loaded" << endl;
    }else if(SceneName == "Lobby"){
        
        for (int i = 0; i < 6; i++) {
            playerUsernames[i].setFont(font);
            playerUsernames[i].setCharacterSize(24);
            playerUsernames[i].setFillColor(sf::Color::Red);
        }
        
        playerUsernames[0].setString("player1");
        playerUsernames[0].setPosition(1500, 300);
        
        playerUsernames[1].setString("player2");
        playerUsernames[1].setPosition(1500, 330);
        
        playerUsernames[2].setString("player3");
        playerUsernames[2].setPosition(1500, 360);
        
        playerUsernames[3].setString("player4");
        playerUsernames[3].setPosition(1500, 390);
        
        playerUsernames[4].setString("player5");
        playerUsernames[4].setPosition(1500, 420);
        
        playerUsernames[5].setString("player6");
        playerUsernames[5].setPosition(1500, 450);
        cout<< "lobby username text loaded" << endl;
        
        countdownTimerText.setFont(font);
        countdownTimerText.setCharacterSize(24);
        countdownTimerText.setString("30");
        countdownTimerText.setPosition(960, 50);
        
        readyButton.setFont(font);
        readyButton.setCharacterSize(40);
        readyButton.setString("Ready!");
        readyButton.setPosition(960, 1000);
    }else if(SceneName == "Racetrack"){
        
        
        for (int i = 0; i<6; i++) {
            lapTimerText[i].setFont(font);
            lapTimerText[i].setString("0:00:000");
            lapTimerText[i].setCharacterSize(40);
            
            lapTimerText[i].setFont(font);
            lapTimerText[i].setString("0:00:000");
            lapTimerText[i].setCharacterSize(40);
            
            lapTimerText[i].setFont(font);
            lapTimerText[i].setString("0:00:000");
            lapTimerText[i].setCharacterSize(40);
            
            lapTimerText[i].setFont(font);
            lapTimerText[i].setString("0:00:000");
            lapTimerText[i].setCharacterSize(40);
            
            lapTimerText[i].setFont(font);
            lapTimerText[i].setString("0:00:000");
            lapTimerText[i].setCharacterSize(40);
        }
        
        lapTimerText[0].setPosition(1300, 300);
        lapTimerText[1].setPosition(1300, 350);
        lapTimerText[2].setPosition(1300, 400);
        lapTimerText[3].setPosition(1300, 450);
        lapTimerText[4].setPosition(1300, 500);
        
    }
}

void Graphics::CheckpointCollected(int checkpointID){
    checkpointSprite[checkpointID].setPosition(-100, -100);
}

void Graphics::LapCompletedResetCheckpoints(){
    checkpointSprite[0].setPosition(1650, 93);
    checkpointSprite[0].setRotation(90);
    checkpointSprite[1].setPosition(1772, 900);
    checkpointSprite[1].setRotation(180);
    checkpointSprite[2].setPosition(350, 1010);
    checkpointSprite[2].setRotation(270);
    checkpointSprite[3].setPosition(970, 450);
    checkpointSprite[4].setPosition(600, 93);
    checkpointSprite[4].setRotation(90);
}



