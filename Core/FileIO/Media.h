#pragma once
#ifndef MEDIA_H
#define MEDIA_H

#include <QObject>
#include <QMediaPlayer>
#include <QMediaRecorder>
#include <QAudioEncoderSettings>
#include <QAudioProbe>
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
    void setAudioVolume(int volume);
    void setAudioPosition(qint64 position)

private:
    QString codecForFileExtension(const QString& extension) const;

    QMediaPlayer* audioPlayer;//ָ���������Ƶ�ļ����ŵ� QMediaPlayer �����ָ��

    QMediaRecorder* mediaRecorder;//ָ���¼��Ƶ���ݵ� QMediaRecorder �����ָ��

    QAudioProbe* audioProbe;//ָ�����������Ƶ���ݵ� QAudioProbe �����ָ��
};

#endif // MEDIA_H
