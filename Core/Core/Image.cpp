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
	texture = new QOpenGLTexture(QImage(path).mirrored().convertToFormat(QImage::Format_RGBA8888), QOpenGLTexture::GenerateMipMaps);
	set_size(Vector2D(texture->width(), texture->height()));
	onPropertyChange(properties["imgPath"]);
}


Image::Image(GameObject* gameObj, const std::string& imgPath,Vector2D size):IBoxResizable(gameObj)
{
	this->imgPath = imgPath;
	this->size = size;
	componentType = IMAGE;
	properties.emplace("imgPath", new Property("imgPath", &(this->imgPath), Property::STRING,this));
	properties.emplace("renderOrder",new Property("renderOrder", &(this->render_order), Property::INT, this));
	if (SceneMgr::get_instance().get_current_scene() != nullptr)
	{
		auto& map = SceneMgr::get_instance().get_current_scene()->getAllGameObjsByDepth();

		if (map.size() == 0)
		{
			render_order = 0;
			map.emplace(render_order, gameObject);
			return;
		}
		auto it = map.end();
		render_order = (--it)->first + 1;
		map.emplace(render_order, gameObject);
	}		
}


Image::~Image()
{
	if (SceneMgr::get_instance().get_current_scene() != nullptr)
	{
		auto& map = SceneMgr::get_instance().get_current_scene()->getAllGameObjsByDepth();
		map.erase(render_order);
	}
}



void Image::serialize(PHString& str)
{
	str.appendLine(to_string((int)componentType));
	str.appendLine(imgPath);
	str.appendLine(size.tostring());
	str.appendLine(to_string(render_order));
}


void Image::set_property(Property* property, void* value)
{
	IBoxResizable::set_property(property, value);
	if (property->get_name() == "imgPath")
	{
		set_imgPath(*(string*)value,false);
	}
	else if (property->get_name() == "size")
	{
		set_size(*(Vector2D*)value);
	}
	else if (property->get_name()== "renderOrder")
	{
		set_render_order(*(int*)value,false);
	}
}

void Image::set_render_order(int order,bool refreshUI)
{
	auto& map = SceneMgr::get_instance().get_current_scene()->getAllGameObjsByDepth();
	if (auto it = map.find(order); it != map.end())
	{
		auto tempObj = it->second;
		it->second = gameObject;
		map[render_order] = tempObj;
		tempObj->getComponent<Image>()->set_render_order_directly(render_order);
		render_order = order;
	}
	else
	{
		map.erase(render_order);
		render_order = order;
		map.emplace(order, gameObject);
	}	
	if (refreshUI && GameEngine::get_instance().needToRefeshUI(gameObject))
	{
		GameEngine::get_instance().onPropertyChange(properties["renderOrder"]);
	}
}

int Image::get_render_order()
{
	return render_order;
}

void Image::deserialize(std::stringstream& ss)
{
	string s;
	getline(ss, s);
	set_imgPath(s);
	getline(ss, s);
	size = Vector2D::fromString(s);
	getline(ss, s);
	render_order = stoi(s);
}

void Image::reset()
{
	imgPath = "";
	size = Vector2D::zero();
}

void Image::set_render_order_directly(int order)
{
	render_order = order;
}
