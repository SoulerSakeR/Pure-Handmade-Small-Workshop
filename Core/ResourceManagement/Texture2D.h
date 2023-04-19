#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#include <qstring.h>
#include <QVector>
#include "Core/Core/ISerializable.h"
#include "qopengltexture.h"
#include <sstream>

struct Color32{
public:
    int r;
    int g;
    int b;
    int a;
    Color32(){}
    Color32(int r, int g, int b, int a)
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
        r = std::stoi(item);
        std::getline(iss, item, ',');
        g = std::stoi(item);
        std::getline(iss, item, ',');
        b = std::stoi(item);
        std::getline(iss, item, ',');
        a = std::stoi(item);
    }
    std::string toString() const
    {
        return std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + "," + std::to_string(a);
    }
    float red() const
    {
		return r / 255.0f;
	}
    float green() const
    {
		return g / 255.0f;
	}
    float blue() const
    {
		return b / 255.0f;
	}
    float alpha() const
    {
		return a / 255.0f;
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
