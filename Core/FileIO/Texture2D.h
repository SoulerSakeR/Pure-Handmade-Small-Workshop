#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#include <qstring.h>
#include <QVector>

struct Color32{
    float r;
    float g;
    float b;
};

class Texture2D
{
public:
   Texture2D();
   void setPixel(int x,int y);
   QVector<QVector<Color32>> data;
   int widthT;
   int heightT;
   QString content;

private:


};
#endif // TEXTURE2D_H
