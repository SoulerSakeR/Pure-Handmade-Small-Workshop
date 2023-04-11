#pragma once
#include "GameLoop.h"
#include "Core/Utils/Vector2D.h"
#include "Core/Core/GameObject.h"
#include "Core/Utils/Vector2D.h"
class Player{
public:
    Player(GameObject* gameObject ,float posX, float posY) // ���캯������ʼ��λ����Ϣ
                                                        // ������ʱ
        :
        gameObject(gameObject),
        m_position(0.0f, 0.0f), 
        m_velocity(0.0f, 0.0f)
    {}

    // �˶���Ϣ
    void move(float deltaTime);
    void updateInfo(float deltatime);

    // ״̬��Ϣ ����Ϸ�����壩
   // int getHealth() ;
   // void setHealth(int health);
   // int getScore() ;
   // void setScore(int score);

    Vector2D getPosition();
    void setMoveValue(Vector2D value);

    Vector2D getVelocity();
    void setVelocity(float Vx, float Vy);


    // ��������Ϸ����Ľ�����Ϣ ����Ϸ�����壩
   // void interactWithEnemy(GameObject& enemy);
   // void interactWithItem(GameObject& item);
   // void interactWithNPC(GameObject& npc);

    int getHealth();
    void setHealth(int health);

    int getScore();
    void setScore(int score);

private:
    Vector2D m_position; // λ����Ϣ ���Ĭ��λ����Ϣ����(0,0)
    Vector2D m_velocity; // �˶���Ϣ

    int m_health;        // ״̬��Ϣ
    int m_score;         // ״̬��Ϣ

    GameObject* gameObject; //ָ��һ����Ϸ����
};
