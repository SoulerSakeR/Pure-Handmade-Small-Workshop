#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#include <qstring.h>
#include <QVector>
#include "Core/Core/ISerializable.h"
#include "qopengltexture.h"
#include <sstream>
#include "PHAsset.h"

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

class Texture2D: public PHAsset, public ISerializable
{
public:
    Texture2D();
   virtual ~Texture2D();
   static Texture2D* CreateTexture2D(const std::string& name, const std::string& absolutePath, bool mipmap=true, int minification_filter=0, int magnification_filter=0, int wrap_mode = 0);
   static Texture2D* loadFromImgPath(const std::string& absolutePath);
   static bool copyTargetImg(const std::string& absolutePath, const std::string& desRelativeDir);

   //getter and setter
   std::string get_name() const;
   bool set_name(const std::string& name);

   std::string get_path() const;
   bool set_path(const std::string& path);

   std::string get_img_path() const;
   bool set_img_path(const std::string& relativePath);

   QOpenGLTexture* get_texture() const;
   bool set_texture(const std::string& absolutePath, bool horizontallyMirrored = false, bool vertically = true);
   bool set_texture(QOpenGLTexture* texture);

   int get_width() const;
   int get_height() const;

   bool get_mipmap() const;
   bool set_mipmap(bool mipmap);

   int get_minification_filter() const;
   bool set_minification_filter(int filter);

   int get_magnification_filter() const;
   bool set_magnification_filter(int filter);

   int get_wrap_mode() const;
   bool set_wrap_mode(int mode);

   void save();
   bool isNull() const;
   void reset_texture();

   // inherited via PHAsset
   virtual Texture2D* loadFromPath(const std::string& path, bool isRelativePath) override;

   // Inherited via ISerializable
   virtual void serialize(PHString& str) override;
   virtual void deserialize(std::stringstream& ss) override;
   virtual void serialize_1_0(PHString& str) override;
   virtual void deserialize_1_0(std::stringstream& ss) override;

   const std::string EXTENSION = ".texture";

private:
    Texture2D(const Texture2D& other) = delete;
    Texture2D& operator=(const Texture2D& other) = delete;

    QOpenGLTexture* texture;
    std::string img_path;       
    bool mipmap;
    int minification_filter;
    int magnification_filter;
    int wrap_mode;

    friend class ResourceMgr;
};
#endif // TEXTURE2D_H
