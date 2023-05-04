#include "Texture2D.h"
#include <Core/Utils/PHPath.h>
#include <Core/SystemStatus/GameEngine.h>
#include "ResourceMgr.h"
#include "Core/FileIO/IO.h"
#include "Core/Utils/PHPath.h"

using namespace std;

Texture2D::Texture2D()
{
	assetType = TEXTURE2D;
	name = "";
	img_path = "";
	texture = nullptr;
	minification_filter = 0;
	magnification_filter = 0;
	wrap_mode = 0;
}

Texture2D::~Texture2D()
{
	if (texture != nullptr)
	{
		delete texture;
	}
	unregisterAssetFromMgr();
}

Texture2D* Texture2D::CreateTexture2D(const std::string& name, const std::string& img_path, bool mipmap, int minification_filter, int magnification_filter, int wrap_mode)
{
	Texture2D* result = new Texture2D();
	if (!result->set_name(name))
	{
		return nullptr;
	}	
	result->set_mipmap(mipmap);
	result->set_minification_filter(minification_filter);
	result->set_magnification_filter(magnification_filter);
	result->set_wrap_mode(wrap_mode);
	result->set_img_path(img_path);
	return result;
}

Texture2D* Texture2D::loadFromImgPath(const std::string& absolutePath)
{
	return CreateTexture2D(PHPath(absolutePath).getFileName(false), absolutePath);
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
	if (isExist(name))
	{
		Debug::logWarning() << "Texture2D with name " << name << " already exist\n";
		return false;
	}
	if (this->name != "")
		unregisterAssetFromMgr();
	this->name = name;
	registerAssetToMgr();
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
	return set_texture(path);
}

QOpenGLTexture* Texture2D::get_texture() const
{
	return texture;
}

bool Texture2D::set_texture(const std::string& absolutePath,bool horizontallyMirrored,bool verticallyMirrored)
{
	QImage image(QString::fromStdString(absolutePath));
	if (image.isNull())
	{
		Debug::logWarning()<< "Texture2D with name " << name << " load image failed, image path: \""<<absolutePath<<"\"\n";
		return false;
	}		
	img_path = absolutePath;
	reset_texture();
	RenderWidget::getCurrentWidget().makeCurrent();
	texture = new QOpenGLTexture(image.mirrored(horizontallyMirrored, verticallyMirrored).convertToFormat(QImage::Format_RGBA8888));
	texture->create();
	RenderWidget::getCurrentWidget().doneCurrent();
	Debug::logInfo() << "Texture2D " << name << " load image success, image path: " << absolutePath << "\n";
	Debug::logInfo() << "Texture2D " << name << " width: " << texture->width() << " height: " << texture->height() << "\n";
	//TODO: 设置纹理参数
	return true;
}

bool Texture2D::set_texture(QOpenGLTexture* texture)
{
	reset_texture();
	this->texture = texture;
	return true;
}

int Texture2D::get_width() const
{
	if(texture!=nullptr)
		return texture->width();
	return 0;
}

int Texture2D::get_height() const
{
	if (texture != nullptr)
		return texture->height();
	return 0;
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

Texture2D* Texture2D::loadFromPath(const std::string& path, bool isRelativePath)
{
	PHPath texturePath(path);
	if (isRelativePath)
	{
		texturePath = PHPath(ResourceMgr::get_instance().getAssetDir()).combinePath(path);
	}
	if (auto res = IO::readText(QString::fromStdString(texturePath.getNewPath())); ! res.isNull())
	{
		auto result = new Texture2D();
		stringstream ss(res.toStdString());
		result->deserialize(ss);
		if (isExist(result->name))
		{
			Debug::logWarning() << " [Texture2D::loadFromPath] Texture2D with name " << result->get_name() << " already exist\n";
		}
		else
		{
			result->registerAssetToMgr();
		}
		return result;
	}
	return nullptr;
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
