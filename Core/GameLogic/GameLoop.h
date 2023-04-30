#pragma once
#define FRAMERATE = 60;
#define TIMEPERFRAME = 16;
#include <iostream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include <Core/SystemStatus/GameEngine.h>
#include "Core/CppcallLua/BindingLua.h"
#include "Player.h"

class RenderWidget;
class Script;

class GameLoop {
    friend class GameEngine;
public:
    GameLoop() { 
    }
    ~GameLoop() {
        delete lua;
    }

    //void gameLoop(GameProject* gamePrj);

    Player* getPlayer();
    void setPlayer(Player* player);

    void startGameLoop();
    void startRenderLoop();

    //spript update functions
    void awake();
    void start();
    void onCollide(const std::vector<CollisonInfo>&);
    void beforeUpdate();
    void update();
    void afterUpdate();


    void updatePlayer(float deltaTime);
        // 更新玩家状态-比如位置和执行动作
         
        // TODO:更新Player的速度
        
        // TODO:更新Player的位置
    

    void updateScene(RenderWidget* aWidget);
        // 更新游戏引擎中的场景信息-（已完成？）

    float updateRender();


    void updateGameState(float deltaTime);
        // 更新游戏状态
        // 比如玩家的位置
        // ...

    void updateScripts(float deltaTime, sol::state &lua);

    void preloadScriptFiles(sol::state& lua);

    void printDataInfo(float deltaTime);
        //输出数据信息-测试用
    

    void updateGame(RenderWidget* gameWidget,sol::state &lua,float dealtaTime);
        // 游戏逻辑更新函数，每帧调用一次
        //updatePlayer(deltaTime);
        //updateScene(Rwg);
        //checkCollisions(deltaTime);
        //updateGameState(deltaTime);
        //printDataInfo(deltaTime);
    void shutdown();

    void setPlayingStatus(bool flag);

    bool isClosed = false;
private:
   Player* player; //绑定一个Player.包括以下功能 
    //1.人物移动（TODO:输入检测） 2.设置人物移动速度 3.获取人物位置 4.设置人物状态 
   
   int gameTime; // 储存srand(GetTickCount())
   bool isRunning = true;
   
   bool isPlaying = false;
   sol::state* lua = nullptr;
   std::vector<GameObject*> gameObjects;
   std::vector<Script*> allScripts;
};
