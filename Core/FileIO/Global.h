#ifndef GLOBAL_H
#define GLOBAL_H
#include "QString"
#include "TextAsset.h"
#include "Texture2D.h"
#include "QImage"
#include <QDir>
#include <QFile>
#include <QDebug>

class Global
{
public:
   bool write(QString content,QString path,int mode);
   bool copy(QString srcPath,QString desPath);
   Texture2D loadTexture2D(QString path);
   QString loadTextAsset(QString path);
   Global();
};
#endif // GLOBAL_H
