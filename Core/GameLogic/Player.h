#pragma once
#include "Core/Utils/Vector2D.h"
#include "Core/Core/GameObject.h"
#include "Core/Utils/Vector2D.h"

#include "Core/Input/InputDetection.h"

class Player {
public:
    Player(GameObject* gameObj,float posX, float posY) // 构造函数，初始化位置信息
        // 现在暂时
        : //绑定某个游戏项目
        gameObject(gameObj),
        m_position(0.0f, 0.0f),
        m_velocity(0.0f, 0.0f)
    {}

    // 输入检测
    static char playerInputDetection(float deltaTime);

    // 运动信息
    void move(float deltaTime);
    void updatePlayerInfo(float deltatime);
    void setMoveValue(Vector2D value);
    Vector2D getVelocity();
    void setVelocity(float Vx, float Vy);

    // 位置信息
    Vector2D getPosition();

    int getHealth();
    void setHealth(int health);

    int getScore();
    void setScore(int score);


    // 状态信息 （游戏待定义）
   // int getHealth() ;
   // void setHealth(int health);
   // int getScore() ;
   // void setScore(int score);

    // 与其他游戏对象的交互信息 （游戏待定义）
   // void interactWithEnemy(GameObject& enemy);
   // void interactWithItem(GameObject& item);
   // void interactWithNPC(GameObject& npc);

private:
    Vector2D m_position; // 位置信息 玩家默认位置信息是在(0,0)
    Vector2D m_velocity; // 速度

    int m_health;        // 血量-游戏状态信息
    int m_score;         // 分数-游戏状态信息

    GameObject* gameObject; //指向一个游戏物体
};