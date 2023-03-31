#define FRAMERATE = 60;
#define TIMEPERFRAME = 16;
#include <iostream>
#include <Core/SystemStatus/GameEngine.h>


class GameLogic {
public:
    GameLogic() {}
    ~GameLogic() {}

    //void gameLoop(GameProject* gamePrj);

    void updatePlayer(float deltaTime) {
        // �������״̬-����λ�ú�ִ�ж���
        
        // ...
    }

    void updateScene(float deltaTime) {
        // ���³�����Ϣ-��Ҫ�͹���;���Խ�
        // ��ȡ������Ϣ(����)���ҽ�����Ⱦ�����磩
        // ...
        
    }

    void checkCollisions(float deltaTime) {
        // �����ײ ����Խ�
        // ...
    }

    void updateGameState(float deltaTime) {
        // ������Ϸ״̬
        // ������ҵ�λ��
        // ...
    }

    void printDataInfo(float deltaTime) {
        //���������Ϣ-������
    }


    void update(float deltaTime) {
        // ��Ϸ�߼����º�����ÿ֡����һ��
        updatePlayer(deltaTime);
        updateScene(deltaTime);
        checkCollisions(deltaTime);
        updateGameState(deltaTime);
        printDataInfo(deltaTime);

    }



 
private:
    // ��ҡ����ˡ���Ϸ״̬�����ݳ�Ա
    // ...
    // ���λ��
    //float position; 
    int gameTime; // ����srand(GetTickCount())
};
