//
//  SceneManager.hpp
//  t01
//
//  Created by FAIRLAMB, JONNY on 12/10/2018.
//  Copyright © 2018 Cordry, Julien. All rights reserved.
//

#pragma once

#ifndef SceneManager_hpp
#define SceneManager_hpp

#include <stdio.h>
#include "Graphics.hpp"

#endif /* SceneManager_hpp */

class SceneManager{
    
public:
    bool map1SceneIsopen = false;
    bool usernameInputSceneIsOpen = true;
    bool mainMenuScreenIsOpen = false;
    bool lobbyScreenIsOpen = false;
    
    std::vector<int> spawnPoints[6];
    
private:
    
    
    
};
