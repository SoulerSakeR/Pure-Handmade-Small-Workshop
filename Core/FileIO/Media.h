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
    static void init(int size);
    static Media* get_Instance()
    {
        if (instances.size() > 0)
        {
            auto instance = instances[instances.size()-1];
            instances.pop_back();
            return instance;
        }          
		else
			return nullptr;
	}
    static void release_Instance(Media* instance)
    {
		instances.push_back(instance);
	}
    Media(QObject* parent = nullptr);
    ~Media();

    bool loadAudio(const QString& inputPath);
    bool playAudio(int loop = 1);
    bool pauseAudio();
    bool stopAudio();
    bool saveAudio(const QString& outputPath);
    void setAudioVolume(float volume);
    void setAudioPosition(qint64 position);

private:
    static std::vector<Media*> instances;
    QString codecForFileExtension(const QString& extension) const;

    QMediaPlayer* audioPlayer;//指向处理加载音频文件播放的 QMediaPlayer 对象的指针

    QMediaRecorder* mediaRecorder;//指向记录音频数据的 QMediaRecorder 对象的指针

    QAudioOutput* audioOutput;//指向处理音频输出的 QAudioOutput 对象的指针
   // QAudioProbe* audioProbe;//指向允许监视音频数据的 QAudioProbe 对象的指针

    friend class GameEngine;
};

#endif // MEDIA_H
