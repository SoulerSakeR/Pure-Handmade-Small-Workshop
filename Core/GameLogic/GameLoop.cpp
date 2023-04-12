#include "GameLoop.h"
using std::chrono::system_clock;


void GameLoop::updatePlayer(float deltaTime) {
        // �������״̬-����λ�ú�ִ�ж���

        // ...
    }

void GameLoop::updateScene(RenderWidget* Rwg) {
        // ���³�����Ϣ-��Ҫ�͹���;���Խ�
        // ��ȡ������Ϣ(����)���ҽ�����Ⱦ�����磩
        // ...       
            // ��Ϸѭ��
        DWORD start_time = GetTickCount();
        DWORD end_time = GetTickCount();
        auto starttime= system_clock::now();
        auto endtime = system_clock::now();
        while (true) {
            // ����ÿһ֡��ʱ����
            
            // �ȴ�һ��ʱ�䣬������Ϸ�ٶ�
            endtime = system_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime);
            if (duration.count() < 10) {
                continue;//Sleep(1 - elapsed_time);
            }
            starttime = system_clock::now();
            Rwg->update();
            endtime = system_clock::now();
            //std::cout << "end_time��" << end_time << "\tstart_time" << start_time << std::endl;
        }

    }

void GameLoop::checkCollisions(float deltaTime) {
        // �����ײ ����Խ�
        // ...
    }

void GameLoop::updateGameState(float deltaTime) {
        // ������Ϸ״̬
        // ������ҵ�λ��
        // ...
    }

void GameLoop::printDataInfo(float deltaTime) {
        //���������Ϣ-������
    }


void GameLoop::update(float deltaTime, RenderWidget* Rwg) {
        // ��Ϸ�߼����º�����ÿ֡����һ��
        updatePlayer(deltaTime);
        updateScene(Rwg);
        checkCollisions(deltaTime);
        updateGameState(deltaTime);
        printDataInfo(deltaTime);
    }


