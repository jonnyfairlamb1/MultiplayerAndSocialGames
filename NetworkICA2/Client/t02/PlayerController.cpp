//
//  PlayerController.cpp
//  t01
//
//  Created by FAIRLAMB, JONNY on 08/10/2018.
//  Copyright © 2018 Cordry, Julien. All rights reserved.
//

#include "PlayerController.hpp"
using namespace std;

#include <iostream>
void PlayerController::PlayerUpdate(sf::Sprite* PlayerCarSprite, NetworkManager* nm)
{
    
    float dt = deltaClock.restart().asSeconds();
    
    //set speed and direction
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && currentSpeed != 0){
        PlayerCarSprite->rotate(-rotateAmount *dt);
    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && currentSpeed != 0){
        PlayerCarSprite->rotate(rotateAmount * dt);
    }
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && general.hasFocus)
    {
        
        if(currentSpeed < maxSpeed)
        {
            currentSpeed += acceleration * dt;
        }
        //rotate direction vector
        sf::Transform t;
        t.rotate(PlayerCarSprite->getRotation());
        movementVec = t.transformPoint(forwardVec);
        isMoving = true;
        
    }else{
        currentSpeed -= deceleration * dt;
        if(currentSpeed <= 0.f){
            currentSpeed = 0.f;
            isMoving = false;
        }else{
            isMoving = true;
        }
    }
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && general.hasFocus)
    {
        currentSpeed -= deceleration * dt;
        if(currentSpeed < 0.f)
            currentSpeed = 0.f;
    }
    
   
    if(general.hasFocus && isMoving){
        PlayerCarSprite->move((movementVec * currentSpeed) * dt);
        playerPosX = PlayerCarSprite->getPosition().x;
        playerPosy = PlayerCarSprite->getPosition().y;
        
        if(nm->isConnected && isMoving) {
            nm->PlayerMovement(PlayerCarSprite->getPosition().x, PlayerCarSprite->getPosition().y, PlayerCarSprite->getRotation()); //needs changing to UDP
            //hasSentMessage = true;
        }
    }
    
//    }else{
//        cout << "Trying to send message while not connected to a server!" << endl;
//    }
}



void PlayerController::GetConstants(General &General){
    general = General;
}
