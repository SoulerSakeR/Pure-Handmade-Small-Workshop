#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include "Media.h"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    MediaTest1 mediaTest;

    mediaTest.loadAudio("path/to/your/audio/file.mp3");
    mediaTest.setAudioVolume(50);
    //����5����
    mediaTest.playAudio();
    QThread::sleep(5);
    //��ͣ2��
    mediaTest.pauseAudio();
    QThread::sleep(2);
    //�ָ����Ų���ʼ������Ƶ�ļ�
    mediaTest.playAudio();
    mediaTest.saveAudio("path/to/output/audio/file.wav");
    //��Ƶ�ļ����������źͱ���10���ӣ�Ȼ��ֹͣ
    QThread::sleep(10);
    mediaTest.stopAudio();

    return app.exec();
}
