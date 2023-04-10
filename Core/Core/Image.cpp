#include "Image.h"
#include "GameObject.h"
#include "Core/FileIO/IO.h"
#include "Core/SystemStatus/GameEngine.h"
#include <Core/ResourceManagement/SceneMgr.h>
using namespace std;

void Image::set_imgPath(const std::string& imgPath)
{
	this->imgPath = imgPath;
	auto texture2D = IO::loadTexture2D(GameEngine::get_instance().getRootPath() + imgPath);
	this->size = Vector2D(texture2D.widthT, texture2D.heightT);
	if (GameEngine::get_instance().getSelectedGameObject() == gameObject)
	{
		GameEngine::get_instance().onPropertyChange(properties[0]);
		GameEngine::get_instance().onPropertyChange(properties[1]);
	}
}

Image::Image(GameObject* gameObj, const std::string& imgPath,Vector2D size):Component(gameObj)
{
	this->imgPath = imgPath;
	this->size = size;
	componentType = IMAGE;
	properties = vector<Property*>();
	properties.push_back(new Property("imgPath", &(this->imgPath), Property::STRING,this));
	properties.push_back(new Property("size", &(this->size), Property::VECTOR2D,this));
	properties.push_back(new Property("renderOrder", &(this->render_order), Property::INT, this));
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

void Image::set_size(Vector2D newSize)
{
	this->size = newSize;
	if (GameEngine::get_instance().getSelectedGameObject() == gameObject)
	{
		GameEngine::get_instance().onPropertyChange(properties[1]);
	}
}


void Image::serialize(PHString& str)
{
	str.appendLine(to_string((int)componentType));
	str.appendLine(imgPath);
	str.appendLine(size.tostring());
	str.appendLine(to_string(render_order));
}

void Image::set_size(float width, float height)
{
	this->size.x = width;
	this->size.y = height;
}

void Image::set_property(Property* property, void* value)
{
	if (property->get_name() == "imgPath")
	{
		set_imgPath(*(string*)value);
	}
	else if (property->get_name() == "size")
	{
		set_size(Vector2D::fromString(((QString*)value)->toStdString()));
	}
	else if (property->get_name()== "renderOrder")
	{
		set_render_order(*(int*)value);
	}
}

void Image::set_render_order(int order)
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
	if (GameEngine::get_instance().needToRefeshUI(gameObject))
	{
		GameEngine::get_instance().onPropertyChange(properties[2]);
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
	imgPath = s;
	getline(ss, s);
	size_t index = s.find(',');
	float x = stof(s.substr(0, index - 1));
	float y = stof(s.substr(index + 1, s.size() - 1));
	size = Vector2D(x, y);
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
