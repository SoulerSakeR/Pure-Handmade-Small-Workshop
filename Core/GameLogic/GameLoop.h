#define FRAMERATE = 60;
#define TIMEPERFRAME = 16;
#include <iostream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include <Core/SystemStatus/GameEngine.h>
#include <Core/Render/renderwidget.h>


class GameLoop {
public:
    GameLoop(RenderWidget* Rwg) {}
    ~GameLoop() {}

    //void gameLoop(GameProject* gamePrj);

    void updatePlayer(float deltaTime);
        // 更新玩家状态-比如位置和执行动作
        
        // ...

    void updateScene(RenderWidget* Rwg);
        // 更新场景信息-需要和光夫哥和敬哥对接
        // 获取场景信息(光夫哥)并且进行渲染（敬哥）
        // ...       


    void checkCollisions(float deltaTime);
        // 检查碰撞 和宇靖对接
        // ...

    void updateGameState(float deltaTime);
        // 更新游戏状态
        // 比如玩家的位置
        // ...

    void printDataInfo(float deltaTime);
        //输出数据信息-测试用
    

    void update(float deltaTime, RenderWidget* Rwg);
        // 游戏逻辑更新函数，每帧调用一次
        //updatePlayer(deltaTime);
        //updateScene(Rwg);
        //checkCollisions(deltaTime);
        //updateGameState(deltaTime);
        //printDataInfo(deltaTime);
 
private:
    // 玩家、敌人、游戏状态等数据成员
    // ...
    // 玩家位置
    //float position; 
    int gameTime; // 储存srand(GetTickCount())
};
