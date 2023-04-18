#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#include <qstring.h>
#include <QVector>
#include "Core/Core/ISerializable.h"
#include "qopengltexture.h"
#include <sstream>

struct Color32{
public:
    float r;
    float g;
    float b;
    float a;
    Color32(){}
    Color32(float r, float g, float b, float a)
    {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
    Color32(std::string string)
    {
        std::istringstream iss(string);
        std::string item;
        std::getline(iss, item, ',');
        r = std::stof(item);
        std::getline(iss, item, ',');
        g = std::stof(item);
        std::getline(iss, item, ',');
        b = std::stof(item);
        std::getline(iss, item, ',');
        a = std::stof(item);
    }
    std::string toString() const
    {
        return std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + "," + std::to_string(a);
    }
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
