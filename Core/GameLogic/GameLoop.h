#define FRAMERATE = 60;
#define TIMEPERFRAME = 16;
#include <iostream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include <Core/SystemStatus/GameEngine.h>
#include <Core/Render/renderwidget.h>
//#include <Core/GameLogic/Player.h>


class GameLoop {
public:
    GameLoop(RenderWidget* Rwg) {}
    ~GameLoop() {}

    //void gameLoop(GameProject* gamePrj);

  //  Player* getPlayer();
 //   void setPlayer(Player* player);

    void updatePlayer(float deltaTime);
        // 更新玩家状态-比如位置和执行动作
        
        // ...

    void updateScene(RenderWidget* Rwg);
        // 更新场景信息-需要和光夫哥和敬哥对接
        // 获取场景信息(光夫哥)并且进行渲染（敬哥）
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
   // Player* player; //绑定一个Player.包括以下功能 
    //1.人物移动（TODO:输入检测） 2.设置人物移动速度 3.获取人物位置 4.设置人物状态 

    int gameTime; // 储存srand(GetTickCount())
};
