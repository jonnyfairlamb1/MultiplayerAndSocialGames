//
//  PlayerController.hpp
//  t01
//
//  Created by FAIRLAMB, JONNY on 08/10/2018.
//  Copyright © 2018 Cordry, Julien. All rights reserved.
//

#ifndef PlayerController_hpp
#define PlayerController_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "SFML/Graphics.hpp"
#include "NetworkManager.hpp"
#include <SFML/System.hpp>
#endif /* PlayerController_hpp */
class PlayerController{
    
private:
    //Movement Values
    sf::Vector2f movementVec;
    const sf::Vector2f forwardVec = sf::Vector2f(1.f, 0.f);
    
    
    const float acceleration = 200.f;
    const float deceleration = 150.f;
    const float maxSpeed = 500.f;
    float rotateAmount = 200.f;
    
    bool hasSentMessage = false;
    bool isMoving = false;
public:
    sf::Clock deltaClock;
    float currentSpeed = 0.f;
    void GetConstants(General &General);
    void PlayerUpdate(sf::Sprite* PlayerCarSprite, NetworkManager* nm);
    float playerPosX = 200;
    float playerPosy = 200;
    
    General general;
};
