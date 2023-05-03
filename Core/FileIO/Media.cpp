#include "Media.h"
#include <qaudiooutput.h>

Media::Media(QObject* parent) : QObject(parent), audioPlayer(nullptr), mediaRecorder(nullptr)//, audioProbe(nullptr)
{
    audioPlayer = new QMediaPlayer(this);
    mediaRecorder = new QMediaRecorder(audioPlayer);
    audioOutput = new QAudioOutput();
    audioPlayer->setAudioOutput(audioOutput);
}

Media::~Media()
{
    delete audioPlayer;
    delete mediaRecorder;
    //delete audioProbe;
}


bool Media::loadAudio(const QString& inputPath)
{
    QString extension = QFileInfo(inputPath).suffix().toLower();
    QString codec = codecForFileExtension(extension);

    if (!codec.isEmpty())
    {
        audioPlayer->setSource(QUrl::fromLocalFile(inputPath));
        return audioPlayer->error() == QMediaPlayer::NoError;
    }
    return false;
}


bool Media::playAudio(bool loop)
{
    audioPlayer->setLoops(loop ? QMediaPlayer::Infinite : 1);
    audioPlayer->play();
    return audioPlayer->error() == QMediaPlayer::NoError;
}

bool Media::pauseAudio()
{
    audioPlayer->pause();
    return audioPlayer->error() == QMediaPlayer::NoError;
}

bool Media::stopAudio()
{
    audioPlayer->stop();
    return audioPlayer->error() == QMediaPlayer::NoError;
}


bool Media::saveAudio(const QString& outputPath)
{
    QString extension = QFileInfo(outputPath).suffix().toLower();
    QString codec = codecForFileExtension(extension);
    return false;
}

void Media::setAudioVolume(float volume)
{
    audioOutput->setVolume(volume);
}
//setAudioPosition��������һ��qint64���͵Ĳ�������ʾ��Ƶ����λ�ã��Ժ���Ϊ��λ��������ʹ�ô˷�������Ƶ����λ������Ϊ�����ֵ����Ҫ������λ������Ϊ��Ƶ�ļ��ĵ�10�룬�����������ã�mediaTest.setAudioPosition(10000)
void Media::setAudioPosition(qint64 position)
{
    audioPlayer->setPosition(position);
}


QString Media::codecForFileExtension(const QString& extension) const
{
    if (extension == "wav")
    {
        return "audio/x-wav";
    }
    else if (extension == "mp3")
    {
        return "audio/mpeg";
    }
    else if (extension == "ogg")
    {
        return "audio/ogg";
    }
    else if (extension == "flac")
    {
        return "audio/x-flac";
    }
    return "";
}
