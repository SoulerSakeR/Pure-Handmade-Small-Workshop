#pragma once
#include "GameLoop.h"
#include "Core/Utils/Vector2D.h"
#include "Core/Physics/GameObject.h"

class Player {
public:
    Player(float x, float y) // 构造函数，初始化位置信息
        : m_position(x, y), m_velocity(0.0f, 0.0f)
    {}

    // 运动信息
    void move(float x, float y, float deltaTime);
    void updateInfo(float deltatime);

    // 状态信息 （游戏待定义）
   // int getHealth() ;
   // void setHealth(int health);
   // int getScore() ;
   // void setScore(int score);

    Vector2D getPosition();
    void setPosition(float x, float y);

    Vector2D getVelocity();
    void setVelocity(float Vx, float Vy);


    // 与其他游戏对象的交互信息 （游戏待定义）
   // void interactWithEnemy(GameObject& enemy);
   // void interactWithItem(GameObject& item);
   // void interactWithNPC(GameObject& npc);

private:
    Vector2D m_position; // 位置信息
    Vector2D m_velocity; // 运动信息
    int m_health;        // 状态信息
    int m_score;         // 状态信息
};
