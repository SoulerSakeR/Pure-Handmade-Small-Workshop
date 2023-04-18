#include "Renderer.h"
#include "Core/ResourceManagement/SceneMgr.h"

using namespace std;

Renderer::Renderer(GameObject* gameObject):Component(gameObject)
{
	componentType = RENDERER;
	properties.emplace("render_order", new Property("render order", &(this->render_order), Property::INT, this));
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

Renderer::~Renderer()
{
	if (SceneMgr::get_instance().get_current_scene() != nullptr)
	{
		auto& map = SceneMgr::get_instance().get_current_scene()->getAllGameObjsByDepth();
		map.erase(render_order);
	}
}

void Renderer::set_property(Property* property, void* value)
{
	Component::set_property(property, value);
	if (property->get_name() == "render_order")
	{
		set_render_order(*(int*)value);
	}
}

void Renderer::reset()
{

}

int Renderer::get_render_order()
{
	return render_order;
}

void Renderer::set_render_order(int order)
{
	auto& map = SceneMgr::get_instance().get_current_scene()->getAllGameObjsByDepth();
	if (auto it = map.find(order); it != map.end())
	{
		auto tempObj = it->second;
		it->second = gameObject;
		map[render_order] = tempObj;
		tempObj->getComponent<Renderer>()->set_render_order_directly(render_order);
		render_order = order;
	}
	else
	{
		map.erase(render_order);
		render_order = order;
		map.emplace(order, gameObject);
	}
	onPropertyChange(properties["render_order"]);
}

void Renderer::set_render_order_directly(int order)
{
	render_order = order;
}

void Renderer::serialize(PHString& str)
{
	str.appendLine(to_string((int)componentType));
	str.appendLine(to_string(render_order));
}

void Renderer::deserialize(std::stringstream& ss)
{
	string line;
	getline(ss, line);
	render_order = stoi(line);
}
