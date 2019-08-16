//
//  Constants.hpp
//  t01
//
//  Created by FAIRLAMB, JONNY on 08/10/2018.
//  Copyright © 2018 Cordry, Julien. All rights reserved.
//
#pragma once
#ifndef General_hpp
#define General_hpp

#include <stdio.h>
#include <iostream>

#endif /* General_hpp */

using namespace std;

class General{
    
public:
    
    static void Start();
    
    std::string playerUsername;
    std::string playerPassword;
    
    std::string visualPlayerPassword;
    int connectionID;
    bool isInLobby = false;
    bool startLobbyTimer = false;
    bool isReady = false;
    bool hasSentMessage = false;
    bool debugSkipToMap = false;
    
    bool enteringUsername = false;
    bool enteringPassword = false;
    bool showErrorText = false;
    bool creatingAccount = false;
    bool showPassword = false;
    bool sentPlayerDetails = false;
    bool isLoggedIn = false;
    
    bool hasFocus = true;
    
    int timerCountdown = 0;
    int playerPos = 0;
    int checkpointsCollected = 0;
    int lapsCompleted = 0;
};


