#ifndef IO_H
#define IO_H

#include <QDir>
#include <QFile>
#include <QDebug>
#include "QString"
#include "TextAsset.h"
#include "QImage"


class IO
{
public:
    static bool createPathIfNotExists(const std::string& directory);
    static bool createPathIfNotExists(const QString& directory);
    static bool write(QString content,QString path,int mode);
    static bool write(const std::string& content,const std::string& path, int mode);
    static bool copy(QString srcPath,QString desPath);
    static std::string readText(const std::string& path);
    static QString readText(const QString& path);
    //static Texture2D loadTexture2D(QString path);
    //static Texture2D loadTexture2D(const std::string& path);
    static QString loadTextAsset(QString path);
private:
	IO(){}
};
#endif // IO_H
