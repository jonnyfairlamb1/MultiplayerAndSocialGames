
//  Client.hpp
//  t01
//
//  Created by FAIRLAMB, JONNY on 17/10/2018.
//  Copyright © 2018 Cordry, Julien. All rights reserved.
//
#pragma once
#ifndef Client_hpp
#define Client_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>

using namespace std;

#endif /* Client_hpp */
class Client{
    
public:
    int posX = 100;
    int posY = 100;
    int rotationAngle;
    bool playerIsReady;
    int connectionID;
    std::string username;
    
    
    
    sf::Sprite thisPlayerSprite;
    sf::Texture thisPlayerTexture;
    sf::Text usernameText;
    sf::Font usernameFont;
    void SetupGraphics(std::string carColour);
    void Disconnected();
    void Update();
    
private:
    
    
    
};
