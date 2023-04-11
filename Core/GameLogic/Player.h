#pragma once
#include "GameLoop.h"
#include "Core/Utils/Vector2D.h"
#include "Core/Core/GameObject.h"
#include "Core/Utils/Vector2D.h"
class Player{
public:
    Player(GameObject* gameObject ,float posX, float posY) // 构造函数，初始化位置信息
                                                        // 现在暂时
        :
        gameObject(gameObject),
        m_position(0.0f, 0.0f), 
        m_velocity(0.0f, 0.0f)
    {}

    // 运动信息
    void move(float deltaTime);
    void updateInfo(float deltatime);

    // 状态信息 （游戏待定义）
   // int getHealth() ;
   // void setHealth(int health);
   // int getScore() ;
   // void setScore(int score);

    Vector2D getPosition();
    void setMoveValue(Vector2D value);

    Vector2D getVelocity();
    void setVelocity(float Vx, float Vy);


    // 与其他游戏对象的交互信息 （游戏待定义）
   // void interactWithEnemy(GameObject& enemy);
   // void interactWithItem(GameObject& item);
   // void interactWithNPC(GameObject& npc);

    int getHealth();
    void setHealth(int health);

    int getScore();
    void setScore(int score);

private:
    Vector2D m_position; // 位置信息 玩家默认位置信息是在(0,0)
    Vector2D m_velocity; // 运动信息

    int m_health;        // 状态信息
    int m_score;         // 状态信息

    GameObject* gameObject; //指向一个游戏物体
};
