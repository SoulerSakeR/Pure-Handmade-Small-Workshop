#include "Texture2D.h"
#include <Core/Utils/PHPath.h>
#include <Core/SystemStatus/GameEngine.h>
#include "ResourceMgr.h"
#include "Core/FileIO/IO.h"

using namespace std;

Texture2D::Texture2D()
{
	name = "";
	img_path = "";
	texture = nullptr;
	minification_filter = 0;
	magnification_filter = 0;
	wrap_mode = 0;
}

Texture2D* Texture2D::loadFromPath(const std::string& absolutePath)
{	
	if (auto res = IO::readText(QString::fromStdString(absolutePath)); !res.isNull())
	{
		auto result = new Texture2D();
		stringstream ss(res.toStdString());
		result->deserialize(ss);
		if (auto it = ResourceMgr::get_instance().texture_assets.find(result->get_name());it != ResourceMgr::get_instance().texture_assets.end())
		{
			Debug::logWarning() << "Texture2D with name " << result->get_name() << " already exist\n";
		}
		else
		{
			ResourceMgr::get_instance().texture_assets[result->get_name()] = result;
		}
		return result;
	}
	return nullptr;
}

Texture2D* Texture2D::loadFromName(const std::string& name)
{
	auto& assets = ResourceMgr::get_instance().texture_assets;
	if (assets.find(name) != assets.end())
	{
		return assets[name];
	}
	else
	{
		Debug::logWarning() << "Texture2D with name " << name << " not exist\n";
		return nullptr;
	}
}

bool Texture2D::isExist(const std::string& name)
{
	return ResourceMgr::get_instance().isTextureExist(name);
}

std::string Texture2D::get_name() const
{
	return name;
}

bool Texture2D::set_name(const std::string& name)
{
	if (name.empty())
	{
		Debug::warningBox(nullptr,"name can't be empty");
		return false;
	}
	if(this->name==name)
		return true;
	if (ResourceMgr::get_instance().isTextureExist(name))
	{
		Debug::logWarning() << "Texture2D with name " << name << " already exist\n";
		return false;
	}
	if (this->name != "")
		ResourceMgr::get_instance().texture_assets.erase(this->name);
	this->name = name;
	ResourceMgr::get_instance().texture_assets[name] = this;	
	return true;
}

std::string Texture2D::get_path() const
{
	if(path.empty())
		return PHPath(ResourceMgr::get_instance().assetPath).combinePath(name+EXTENSION).getNewPath();
	return path;
}

bool Texture2D::set_path(const std::string& path)
{
	this->path = path;
	return true;
}

std::string Texture2D::get_img_path() const
{
	return img_path;
}

bool Texture2D::set_img_path(const std::string& path)
{
	this->img_path = path;
	set_texture(path);
	return true;
}

QOpenGLTexture* Texture2D::get_texture() const
{
	return texture;
}

bool Texture2D::set_texture(const std::string& absolutePath)
{
	QImage image(QString::fromStdString(absolutePath));
	auto w = image.width();
	auto h = image.height();
	if (image.isNull())
	{
		Debug::logWarning()<< "Texture2D with name " << name << " load image failed, image path: \""<<absolutePath<<"\"\n";
		return false;
	}		
	reset_texture();
	texture = new QOpenGLTexture(image.mirrored().convertToFormat(QImage::Format_RGBA8888));
	w = texture->width();
	h = texture->height();
	return true;
}

bool Texture2D::set_texture(QOpenGLTexture* texture)
{
	reset_texture();
	this->texture = texture;
	return true;
}

bool Texture2D::get_mipmap() const
{
	return mipmap;
}

bool Texture2D::set_mipmap(bool mipmap)
{
	this->mipmap = mipmap;
	return true;
}

int Texture2D::get_minification_filter() const
{
	return minification_filter;
}

bool Texture2D::set_minification_filter(int filter)
{
	this->minification_filter = filter;
	return true;
}

int Texture2D::get_magnification_filter() const
{
	return magnification_filter;
}

bool Texture2D::set_magnification_filter(int filter)
{
	this->magnification_filter = filter;
	return true;
}

int Texture2D::get_wrap_mode() const
{
	return wrap_mode;
}

bool Texture2D::set_wrap_mode(int mode)
{
	this->wrap_mode = mode;
	return true;
}

void Texture2D::save()
{
	
	PHString str;
	serialize(str);
	auto path = PHPath(GameEngine::get_instance().getGamePath()).combinePath(get_path());
	IO::write(str.str(),path.getNewPath(),1);
}

bool Texture2D::isNull() const
{
	return texture == nullptr;
}

void Texture2D::reset_texture()
{
	if (texture != nullptr)
	{
		texture->release();
		texture->destroy();
		delete texture;
		texture = nullptr;
	}
}

void Texture2D::serialize(PHString& str)
{
	str.appendLine(name);
	str.appendLine(path);
	str.appendLine(img_path);
	str.appendLine(to_string(mipmap));
	str.appendLine(to_string(minification_filter));
	str.appendLine(to_string(magnification_filter));
	str.appendLine(to_string(wrap_mode));
}

void Texture2D::deserialize(std::stringstream& ss)
{
	string line;
	getline(ss, line);
	name = line;
	getline(ss, line);
	set_path(line);
	getline(ss, line);
	set_img_path(line);
	getline(ss, line);
	set_mipmap(stoi(line));
	getline(ss, line);
	set_minification_filter(stoi(line));
	getline(ss, line);		
	set_magnification_filter(stoi(line));
	getline(ss, line);
	set_wrap_mode(stoi(line));
}
