//
//  main.cpp
//  t01
//
//  Created by Cordry, Julien on 30/09/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#include <SFML/Graphics.hpp>
#include "Graphics.hpp"
#include "NetworkManager.hpp"
#include "PlayerController.hpp"
#include "SceneManager.hpp"
#include "General.hpp"

using namespace std;
#include <iostream>
#include <thread>

int main(){
    //instances of the class
    Queue<vector<std::string>> q;
    Graphics graphics(std::ref(q));
    NetworkManager nm(std::ref(q));
    General general;
    SceneManager sm;
    PlayerController pc;
    
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!");
    sf::Clock delta_clock;
    sf::Clock lapTimer;
    std::thread recvthread;
    
    //setup start points
    sm.spawnPoints[0] = {940, 90};
    sm.spawnPoints[1] = {900, 150};
    sm.spawnPoints[2] = {860, 90};
    sm.spawnPoints[3] = {820, 150};
    sm.spawnPoints[4] = {780, 90};
    sm.spawnPoints[5] = {740, 150};
    
    
    graphics.GetClasses(general);
    pc.GetConstants(general);
    //checks if the debug mode is enabled
    if(general.debugSkipToMap){
        graphics.BuildMap1Scene();
        sm.map1SceneIsopen = true;
    }
    
    //builds the login screen on start
    if(sm.usernameInputSceneIsOpen){
        graphics.BuildUsernameInputScene();
        nm.GetConstantClass(general, sm);
        nm.ConnectTCP();
        recvthread = std::thread(&NetworkManager::TCPRecieve, std::ref(nm));
    }
    
    while (window.isOpen())
    {
        //sets the framerate to the window title
        sf::Time dt = delta_clock.restart();
        int framerate = 1/dt.asSeconds();
        window.setTitle("FPS: " + std::to_string(framerate));
        
        //cout << std::to_string(sf::Mouse::getPosition(window).x) + "---" +std::to_string(sf::Mouse::getPosition(window).y) << endl;
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
            }
            
            if(event.type == sf::Event::GainedFocus){
                general.hasFocus = true;
            }
            if(event.type == sf::Event::LostFocus){
                general.hasFocus = false;
            }
            
            if(event.type == sf::Event::TextEntered && sm.usernameInputSceneIsOpen && general.enteringUsername){
                general.playerUsername += event.text.unicode;
                graphics.playerUsernameText.setString(general.playerUsername);
            }
            
            if(event.type == sf::Event::TextEntered && sm.usernameInputSceneIsOpen && general.enteringPassword){
                general.playerPassword += event.text.unicode;
                if(!general.showPassword){
                    general.visualPlayerPassword += "*";
                }
                graphics.passWordText.setString(general.visualPlayerPassword);
            }
            
            if(sm.usernameInputSceneIsOpen && (event.key.code == sf::Keyboard::Return)){
                sm.usernameInputSceneIsOpen = false;
                sm.mainMenuScreenIsOpen = true;
                graphics.BuildMainMenuScene();
            }
        }
        
        if(!q.IsEmpty()){
            graphics.HandleQueue(nm.playerList);
        }
        
        //----------------------------------------
        //MAIN MENU
        if(sm.mainMenuScreenIsOpen){
            
            sf::FloatRect joinGameCollision = graphics.joinGameText.getGlobalBounds();
            sf::FloatRect quitGameCollion = graphics.quitGameText.getGlobalBounds();
            
            sf::Vector2f point;
            point.x = sf::Mouse::getPosition(window).x;
            point.y = sf::Mouse::getPosition(window).y;
            
            if(joinGameCollision.contains(point) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                if(!general.debugSkipToMap){
                    //dataHandler.GetClasses(general, graphics);
                    
                    sm.mainMenuScreenIsOpen = false;
                    //go to lobby screen
                    sm.lobbyScreenIsOpen = true;
                    nm.RegisterWithServer();
                    
                    graphics.BuildLobbyScene();
                }else{
                    graphics.BuildMap1Scene();
                    sm.map1SceneIsopen = true;
                    sm.mainMenuScreenIsOpen = false;
                }
            }
            if(quitGameCollion.contains(point) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                window.close();
            }
        }
        //----------------------------------------
        //LOGIN SCREEN
        if(sm.usernameInputSceneIsOpen){
            sf::FloatRect userNameCollider = graphics.playerUsernameText.getGlobalBounds();
            sf::FloatRect passwordCollider = graphics.passWordText.getGlobalBounds();
            sf::FloatRect backButton = graphics.backText.getGlobalBounds();
            sf::FloatRect confirmButtonCollider = graphics.confirmText.getGlobalBounds();
            sf::FloatRect createAccountCollider = graphics.createAccountText.getGlobalBounds();
            
            sf::Vector2f point;
            point.x = sf::Mouse::getPosition(window).x;
            point.y = sf::Mouse::getPosition(window).y;
            
            if(userNameCollider.contains(point) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                general.enteringUsername = true;
                general.enteringPassword = false;
                graphics.playerUsernameText.setString("");
                general.playerUsername = "";
            }
            
            if(passwordCollider.contains(point) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                general.enteringUsername = false;
                general.enteringPassword = true;
                graphics.passWordText.setString("");
                general.playerPassword = "";
            }
            
            if(createAccountCollider.contains(point) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                general.creatingAccount = true;
            }
            
            if(confirmButtonCollider.contains(point) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                if(!general.sentPlayerDetails && general.creatingAccount){
                    general.creatingAccount = false;
                    general.sentPlayerDetails = true;
                    nm.AccountData(general.playerUsername, general.playerPassword);
                }else if(!general.creatingAccount && !general.sentPlayerDetails){
                    general.sentPlayerDetails = true;
                    nm.LoginRequest(general.playerUsername, general.playerPassword);
                    cout << "confirm button pressed!" << endl;
                }
            }
            
            if(backButton.contains(point) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                general.creatingAccount = false;
            }
            
            if(nm.isLoggedIn){
                cout << "player is now logged in!" << endl;
                sm.mainMenuScreenIsOpen = true;
                sm.usernameInputSceneIsOpen = false;
                graphics.BuildMainMenuScene();
            }
        }
        
        
        //----------------------------------------
        //LOBBY
        if(sm.lobbyScreenIsOpen){
            sf::FloatRect leftArrow = graphics.arrowLeft.getGlobalBounds();
            sf::FloatRect rightArrow = graphics.arrowRight.getGlobalBounds();
            sf::FloatRect readyButton = graphics.readyButton.getGlobalBounds();
            
            sf::Vector2f point;
            point.x = sf::Mouse::getPosition(window).x;
            point.y = sf::Mouse::getPosition(window).y;
            
            if(leftArrow.contains(point) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                if(graphics.selectedCar == 0){
                    graphics.selectedCar = 5;
                }
                graphics.selectedCar--;
            }else if(rightArrow.contains(point) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                if(graphics.selectedCar == 5 ){
                    graphics.selectedCar = 0;
                }
                graphics.selectedCar++;
            }else if(readyButton.contains(point) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                //change my font colour
                //send ready message to server
                if(!general.isReady){
                    nm.ReadyPlayer();
                    general.isReady = true;
                    for (int i = 0; i < 6; i++) {
                        if(graphics.playerUsernames[i].getString() == general.playerUsername){
                            graphics.playerUsernames[i].setFillColor(sf::Color::Green);
                        }
                    }
                }
            }
            graphics.carSelectionSprite.setTexture(graphics.carSelectionTex[graphics.selectedCar]);
            
            if(nm.gameHasStarted){
                sm.lobbyScreenIsOpen = false;
                sm.map1SceneIsopen = true;
                graphics.BuildMap1Scene();
                lapTimer.restart();
                
                for (int i = 0; i< nm.playerList.size(); i++) {
                    nm.playerList[i].SetupGraphics("black");
                }
                
            }
            //general.timerCountdown = dataHandler.tempCountDownTimer;
            
        }
        //----------------------------------------
        //MAP1
        if(sm.map1SceneIsopen && nm.isConnected){
            pc.PlayerUpdate(&graphics.playerSprite, &nm);
        }else if(sm.map1SceneIsopen && general.debugSkipToMap){
            pc.PlayerUpdate(&graphics.playerSprite, &nm);
        }
        
        if(sm.map1SceneIsopen){
            sf::Time elapsed = lapTimer.getElapsedTime();
            float timer = elapsed.asSeconds();
            graphics.lapTimerText[0].setString(std::to_string(timer));
            
            sf::FloatRect checkpoint1 = graphics.checkpointSprite[0].getGlobalBounds();
            sf::FloatRect checkpoint2 = graphics.checkpointSprite[1].getGlobalBounds();
            sf::FloatRect checkpoint3 = graphics.checkpointSprite[2].getGlobalBounds();
            sf::FloatRect checkpoint4 = graphics.checkpointSprite[3].getGlobalBounds();
            sf::FloatRect checkpoint5 = graphics.checkpointSprite[4].getGlobalBounds();
            sf::FloatRect startLine = graphics.rect.getGlobalBounds();
            sf::FloatRect playerCollider = graphics.playerSprite.getGlobalBounds();
            sf::FloatRect networkPlayersCollider[5];
            
            for (int i = 0; i < nm.playerList.size(); i++) {
                networkPlayersCollider[i] = nm.playerList[i].thisPlayerSprite.getGlobalBounds();
            }
            for (int i = 0; i < 5; i++) {
                if(playerCollider.intersects(networkPlayersCollider[i])){
                    
                    //collision physicas
                    //cout << "Collided with a player!" << endl;
                    
                    
                    
                }
            }
            if(playerCollider.intersects(checkpoint1)){
                general.checkpointsCollected++;
                nm.CheckpointCollected();
                graphics.CheckpointCollected(0);
            }else if(playerCollider.intersects(checkpoint2)){
                general.checkpointsCollected++;
                nm.CheckpointCollected();
                graphics.CheckpointCollected(1);
                
            }else if(playerCollider.intersects(checkpoint3)){
                general.checkpointsCollected++;
                nm.CheckpointCollected();
                graphics.CheckpointCollected(2);
                
            }else if(playerCollider.intersects(checkpoint4)){
                general.checkpointsCollected++;
                nm.CheckpointCollected();
                graphics.CheckpointCollected(3);
                
            }else if(playerCollider.intersects(checkpoint5)){
                general.checkpointsCollected++;
                nm.CheckpointCollected();
                graphics.CheckpointCollected(4);
            }
            if(playerCollider.intersects(startLine) && general.checkpointsCollected == 5){
                general.checkpointsCollected = 0;
                graphics.LapCompletedResetCheckpoints();
                general.lapsCompleted++;
                graphics.lapTimerText[general.lapsCompleted].setString(std::to_string(timer));
                elapsed = lapTimer.restart();
            }
        }
        
        //----------------------------------------
        
        if(!nm.isConnected && !nm.errorHasBeenSent){
            cout << "Disconnected From Server!" << endl;
            nm.errorHasBeenSent = true;
        }
        //            }else if(!nm.isConnected && nm.errorHasBeenSent){
        //                sm.mainMenuScreenIsOpen = true;
        //                sm.lobbyScreenIsOpen = false;
        //                sm.map1SceneIsopen = false;
        //                recvthread.detach();
        //            }
        
        
        //----------------------------------------
        //RENDERING
        window.clear(sf::Color::Black);
        //draw all the graphics objects here
        if(sm.map1SceneIsopen){
            window.draw(graphics.raceTrackSprite);
            window.draw(graphics.backgroundColliderSprite);
            window.draw(graphics.playerSprite);
                        
            for (int i = 0; i<5; i++) {
                window.draw(graphics.lapTimerText[i]);
                window.draw(graphics.checkpointSprite[i]);
            }
            
            for (int i = 0; i< nm.playerList.size(); i++) {
                window.draw(nm.playerList[i].thisPlayerSprite);
            }            
            window.draw(graphics.rect); // rect is the start/finish line
        }
        
        if(sm.usernameInputSceneIsOpen){
            window.draw(graphics.playerUsernameText);
            window.draw(graphics.passWordText);
            window.draw(graphics.backText);
            window.draw(graphics.confirmText);
            window.draw(graphics.errorText);
            
            
            if(!general.creatingAccount){
                window.draw(graphics.createAccountText);
            }
        }
        
        if(sm.mainMenuScreenIsOpen){
            window.draw(graphics.joinGameText);
            window.draw(graphics.quitGameText);
        }
        
        if(sm.lobbyScreenIsOpen){
            window.draw(graphics.arrowRight);
            window.draw(graphics.arrowLeft);
            window.draw(graphics.carSelectionSprite);
            window.draw(graphics.countdownTimerText);
            window.draw(graphics.readyButton);
            
            window.draw(graphics.playerUsernames[0]);
            window.draw(graphics.playerUsernames[1]);
            window.draw(graphics.playerUsernames[2]);
            window.draw(graphics.playerUsernames[3]);
            window.draw(graphics.playerUsernames[4]);
            window.draw(graphics.playerUsernames[5]);
        }
        window.display();
        
        try {
            recvthread.detach();
        } catch (exception ex) {
            
        }
    }
    return 0;
}
