#include "Image.h"
#include "GameObject.h"
#include "Core/FileIO/IO.h"
#include "Core/SystemStatus/GameEngine.h"
#include <Core/ResourceManagement/SceneMgr.h>
using namespace std;

const std::string& Image::get_imgPath()
{
	return imgPath;
}

void Image::set_imgPath(const std::string& imgPath,bool refreshUI)
{
	this->imgPath = imgPath;
	QString path = QString::fromStdString(GameEngine::get_instance().getRootPath() + imgPath);
	if(img!=nullptr)
		delete img;
	img = new QImage(path);
	if (!img->isNull())
	{
		img->convertToFormat(QImage::Format_RGBA8888);
		if (texture != nullptr)
		{
			texture->destroy();
			delete texture;
			texture = nullptr;
		}
		set_size(Vector2D(img->width(), img->height()));
	}
	else
	{
		delete img;
		img = nullptr;
		if (texture != nullptr)
		{
			texture->destroy();
			delete texture;
			texture = nullptr;
		}
	}
	onPropertyChange(properties["img path"]);
}

QImage* Image::get_img()
{
	return img;
}


Image::Image(GameObject* gameObj, const std::string& imgPath,Vector2D size):IBoxResizable(gameObj)
{
	this->imgPath = imgPath;
	this->size = size;
	img = nullptr;
	componentType = IMAGE;
	properties.emplace("img path", new Property("img path", &(this->imgPath), Property::STRING,this));
}

void Image::serialize(PHString& str)
{
	str.appendLine(to_string((int)componentType));
	IBoxResizable::serialize(str);
	str.appendLine(imgPath);
}


void Image::set_property(Property* property, void* value)
{
	IBoxResizable::set_property(property, value);
	if (property->get_name() == "img path")
	{
		set_imgPath(*(string*)value,false);
	}
	else if (property->get_name() == "size")
	{
		set_size(*(Vector2D*)value);
	}
}



void Image::deserialize(std::stringstream& ss)
{
	IBoxResizable::deserialize(ss);
	string s;
	getline(ss, s);
	set_imgPath(s);
}

void Image::reset()
{
	imgPath = "";
	size = Vector2D::zero();
}

