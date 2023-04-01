#ifndef IO_H
#define IO_H
#include "QString"
#include "TextAsset.h"
#include "Texture2D.h"
#include "QImage"
#include <QDir>
#include <QFile>
#include <QDebug>

class IO
{
public:
   static bool write(QString content,QString path,int mode);
   static bool write(const std::string& content,const std::string& path, int mode);
   static bool copy(QString srcPath,QString desPath);
   static Texture2D loadTexture2D(QString path);
   static QString loadTextAsset(QString path);
private:
	IO(){}
};
#endif // IO_H
