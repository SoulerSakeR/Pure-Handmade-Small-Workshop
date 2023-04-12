#pragma once
#include "GameLoop.h"
#include "Core/Utils/Vector2D.h"
#include "Core/Physics/GameObject.h"

class Player {
public:
    Player(float x, float y) // ���캯������ʼ��λ����Ϣ
        : m_position(x, y), m_velocity(0.0f, 0.0f)
    {}

    // �˶���Ϣ
    void move(float x, float y, float deltaTime);
    void updateInfo(float deltatime);

    // ״̬��Ϣ ����Ϸ�����壩
   // int getHealth() ;
   // void setHealth(int health);
   // int getScore() ;
   // void setScore(int score);

    Vector2D getPosition();
    void setPosition(float x, float y);

    Vector2D getVelocity();
    void setVelocity(float Vx, float Vy);


    // ��������Ϸ����Ľ�����Ϣ ����Ϸ�����壩
   // void interactWithEnemy(GameObject& enemy);
   // void interactWithItem(GameObject& item);
   // void interactWithNPC(GameObject& npc);

private:
    Vector2D m_position; // λ����Ϣ
    Vector2D m_velocity; // �˶���Ϣ
    int m_health;        // ״̬��Ϣ
    int m_score;         // ״̬��Ϣ
};
