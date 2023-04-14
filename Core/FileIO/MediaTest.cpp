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
    //播放5秒钟
    mediaTest.playAudio();
    QThread::sleep(5);
    //暂停2秒
    mediaTest.pauseAudio();
    QThread::sleep(2);
    //恢复播放并开始保存音频文件
    mediaTest.playAudio();
    mediaTest.saveAudio("path/to/output/audio/file.wav");
    //音频文件将继续播放和保存10秒钟，然后停止
    QThread::sleep(10);
    mediaTest.stopAudio();

    return app.exec();
}
