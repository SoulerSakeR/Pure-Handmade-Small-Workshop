#include "GameLoop.h"



    void updatePlayer(float deltaTime) {
        // 更新玩家状态-比如位置和执行动作

        // ...
    }

    void updateScene(RenderWidget* Rwg) {
        // 更新场景信息-需要和光夫哥和敬哥对接
        // 获取场景信息(光夫哥)并且进行渲染（敬哥）
        // ...       
            // 游戏循环
        while (true) {
            // 计算每一帧的时间间隔
            DWORD start_time = GetTickCount();

            // 等待一段时间，控制游戏速度
            DWORD end_time = GetTickCount();
            DWORD elapsed_time = end_time - start_time;
            if (elapsed_time < 16) {
                Sleep(16 - elapsed_time);
            }
            std::cout << "end_time：" << end_time << "\tstart_time" << start_time << std::endl;
            Rwg->update();
        }

    }

    void checkCollisions(float deltaTime) {
        // 检查碰撞 和宇靖对接
        // ...
    }

    void updateGameState(float deltaTime) {
        // 更新游戏状态
        // 比如玩家的位置
        // ...
    }

    void printDataInfo(float deltaTime) {
        //输出数据信息-测试用
    }


        void update(float deltaTime, RenderWidget* Rwg) {
        // 游戏逻辑更新函数，每帧调用一次
        updatePlayer(deltaTime);
        updateScene(Rwg);
        checkCollisions(deltaTime);
        updateGameState(deltaTime);
        printDataInfo(deltaTime);
    }


