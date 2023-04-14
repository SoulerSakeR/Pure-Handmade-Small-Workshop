#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#include <qstring.h>
#include <QVector>
#include "Core/Core/ISerializable.h"
#include "qopengltexture.h"

struct Color32{
    float r;
    float g;
    float b;
    float a;
};

class Texture2D: public ISerializable
{
public:
   Texture2D(const std::string& path);
   
   // Inherited via ISerializable
   virtual void serialize(PHString& str) override;
   virtual void deserialize(std::stringstream& ss) override;
private:
    QOpenGLTexture* texture;
    std::string path;    
    std::string name;
};
#endif // TEXTURE2D_H
