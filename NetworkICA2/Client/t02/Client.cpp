//
//  Client.cpp
//  t01
//
//  Created by FAIRLAMB, JONNY on 17/10/2018.
//  Copyright © 2018 Cordry, Julien. All rights reserved.
//
#include <iostream>
#include "Client.hpp"
using namespace std;

void Client::Disconnected(){
    posX = -100;
    posY = -100;
}

void Client::SetupGraphics(std::string carColour){
    if(!thisPlayerTexture.loadFromFile("/Users/q5058597/Desktop/NetworkICA2/Client/t02/Artwork/car_blue_1.png")){
        std::cout <<"Client car Texture didnt load!" <<endl;
    }
    thisPlayerSprite.setTexture(thisPlayerTexture);
    std::cout << "Client car loaded" << endl;
    
    if(!usernameFont.loadFromFile("/Users/q5058597/Desktop/NetworkICA2/Client/t02/Artwork/arial.ttf")){
        cout<<"could not load font file"<<endl;
    }
    
    usernameText.setFont(usernameFont);
    usernameText.setString(username);
    usernameText.setCharacterSize(24);
    usernameText.setFillColor(sf::Color::Red);
    
    usernameText.setPosition(thisPlayerSprite.getPosition().x, thisPlayerSprite.getPosition().y/2);
    cout<<"Networked player loaded!"<<endl;
}

void Client::Update(){
    //cout<<"Updating player movement!"<<endl;
    thisPlayerSprite.setPosition(posX, posY);
    thisPlayerSprite.setRotation(rotationAngle);
}
