//
//  Graphics.hpp
//  t01
//
//  Created by FAIRLAMB, JONNY on 08/10/2018.
//  Copyright © 2018 Cordry, Julien. All rights reserved.
//

#pragma once

#ifndef Graphics_hpp
#define Graphics_hpp


#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "SceneManager.hpp"
#include "Client.hpp"
#include "General.hpp"
#include "Queue.hpp"

#endif /* Graphics_hpp */
class Graphics{
    
public:
    Graphics(Queue<vector<string>> &q);
    
    void GetClasses(General &General);
    void BuildUsernameInputScene();
    void BuildMap1Scene();
    void BuildMainMenuScene();
    void BuildLobbyScene();
    void HandleQueue(std::vector<Client> PlayerList);
    
    void CheckpointCollected(int checkpointID);
    void LapCompletedResetCheckpoints();
    
    sf::RectangleShape rect;
    
    //Map1
    sf::Texture raceTrackTex;
    sf::Texture backgroundColliderTex;
    sf::Texture playerTex;
    sf::Sprite raceTrackSprite;
    sf::Sprite backgroundColliderSprite;
    sf::Sprite playerSprite;
    sf::Text lapTimerText[5];
    sf::Text playerPosition;
    
    sf::Texture checkpointTex;
    sf::Sprite checkpointSprite[5];
    
        
    //username input
    sf::Text playerUsernameText;
    sf::Text passWordText;
    sf::Text confirmText;
    sf::Text backText;
    sf::Text errorText;
    sf::Text createAccountText;
    
    //mainMenu
    sf::Text joinGameText;
    sf::Text quitGameText;
    
    //lobbyScene
    sf::Texture arrow;
    sf::Sprite arrowLeft;
    sf::Sprite arrowRight;
    sf::Texture carSelectionTex[6];
    sf::Sprite carSelectionSprite;
    
    sf::Text playerUsernames[6];
    sf::Text countdownTimerText;
    sf::Text readyButton;
    int selectedCar = 1;
private:
    
    void LoadBackground();
    void LoadPlayer();
    void LoadText(std::string SceneName);
    
    void LobbyUsernames(vector<std::string> data);
    void UpdateTimerGraphics(vector<std::string> data);
    void UpdateWaitingforPlayersText(vector<std::string> data);
    void PlayerReady(vector<std::string> data, std::vector<Client> PlayerList);
    void AccountCreated();
    void AccountExists();
    void Error(vector<std::string> data);
    sf::Font font;
    General general;
    
    
    string DataPath = "/⁨Users⁩/⁨jonnyfairlamb⁩/⁨Desktop⁩/⁨NetworkICA2⁩/Client⁩/t02⁩/Artwork/";
    //Macbook datapath = /⁨Users⁩/⁨jonnyfairlamb⁩/⁨Desktop⁩/⁨NetworkICA2⁩/Client⁩/t02⁩/Artwork/
    //Uni Datapath = /Users/q5058597/Desktop/NetworkICA2/Client/t02/Artwork/
    
    Queue<vector<std::string>> &queue;
};
