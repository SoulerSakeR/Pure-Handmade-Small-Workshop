#pragma once
#pragma once
#ifndef MEDIA_H
#define MEDIA_H

#include <QObject>
#include <QMediaPlayer>
#include <QMediaRecorder>
//#include <QAudioEncoderSettings>
//#include <QAudioProbe>
#include <QUrl>
#include <QString>
#include <QFileInfo>

class Media : public QObject
{
    Q_OBJECT

public:
    Media(QObject* parent = nullptr);
    ~Media();

    bool loadAudio(const QString& inputPath);
    bool playAudio();
    bool pauseAudio();
    bool stopAudio();
    bool saveAudio(const QString& outputPath);
    void setAudioVolume(float volume);
    void setAudioPosition(qint64 position);

private:
    QString codecForFileExtension(const QString& extension) const;

    QMediaPlayer* audioPlayer;//指向处理加载音频文件播放的 QMediaPlayer 对象的指针

    QMediaRecorder* mediaRecorder;//指向记录音频数据的 QMediaRecorder 对象的指针

    QAudioOutput* audioOutput;//指向处理音频输出的 QAudioOutput 对象的指针
   // QAudioProbe* audioProbe;//指向允许监视音频数据的 QAudioProbe 对象的指针
};

#endif // MEDIA_H
