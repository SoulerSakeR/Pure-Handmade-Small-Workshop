#pragma once
#include "Core/Utils/Vector2D.h"
#include "Core/Core/GameObject.h"
#include "Core/Utils/Vector2D.h"

class Player {
public:
    Player(GameObject* gameObject, float posX, float posY) // ���캯������ʼ��λ����Ϣ
        // ������ʱ
        :
        gameObject(gameObject), //��ĳ����Ϸ��Ŀ
        m_position(0.0f, 0.0f),
        m_velocity(0.0f, 0.0f)
    {}

    // ������
    void playerInputDetection(float deltaTime);

    // �˶���Ϣ
    void move(float deltaTime);
    void updatePlayerInfo(float deltatime);
    void setMoveValue(Vector2D value);
    Vector2D getVelocity();
    void setVelocity(float Vx, float Vy);

    // λ����Ϣ
    Vector2D getPosition();

    int getHealth();
    void setHealth(int health);

    int getScore();
    void setScore(int score);


    // ״̬��Ϣ ����Ϸ�����壩
   // int getHealth() ;
   // void setHealth(int health);
   // int getScore() ;
   // void setScore(int score);

    // ��������Ϸ����Ľ�����Ϣ ����Ϸ�����壩
   // void interactWithEnemy(GameObject& enemy);
   // void interactWithItem(GameObject& item);
   // void interactWithNPC(GameObject& npc);

private:
    Vector2D m_position; // λ����Ϣ ���Ĭ��λ����Ϣ����(0,0)
    Vector2D m_velocity; // �ٶ�

    int m_health;        // Ѫ��-��Ϸ״̬��Ϣ
    int m_score;         // ����-��Ϸ״̬��Ϣ

    GameObject* gameObject; //ָ��һ����Ϸ����
};