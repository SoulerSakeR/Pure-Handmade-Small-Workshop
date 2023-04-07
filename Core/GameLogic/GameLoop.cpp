#include "GameLoop.h"



void GameLoop::updatePlayer(float deltaTime) {
        // �������״̬-����λ�ú�ִ�ж���

        // ...
    }

void GameLoop::updateScene(RenderWidget* Rwg) {
        // ���³�����Ϣ-��Ҫ�͹���;���Խ�
        // ��ȡ������Ϣ(����)���ҽ�����Ⱦ�����磩
        // ...       
            // ��Ϸѭ��
        std::cout << "update scene" << std::endl;
        while (true) {
            // ����ÿһ֡��ʱ����
            DWORD start_time = GetTickCount();

            // �ȴ�һ��ʱ�䣬������Ϸ�ٶ�
            DWORD end_time = GetTickCount();
            DWORD elapsed_time = end_time - start_time;
            if (elapsed_time < 16) {
                Sleep(16 - elapsed_time);
            }
            std::cout << "end_time��" << end_time << "\tstart_time" << start_time << std::endl;
            Rwg->update();
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


