#include "Renderer.h"
#include "Core/ResourceManagement/SceneMgr.h"

using namespace std;

Renderer::Renderer(GameObject* gameObject):Component(gameObject)
{
	componentType = RENDERER;
	render_layer = 0;
	properties.emplace("render layer", new Property("render layer", &(this->render_layer), Property::COMBO_BOX, this));
	if (auto& sceneMgr = SceneMgr::get_instance();sceneMgr.hasCurrentScene())
	{
		sceneMgr.registerRenderer(this);
	}
}

Renderer::~Renderer()
{
	SceneMgr::get_instance().get_render_setting()->unregisterRenderer(this);
}

void Renderer::set_property(Property* property, void* value)
{
	Component::set_property(property, value);
	if (property->get_name() == "render layer")
	{
		set_render_layer(*(int*)value);
	}
}

void Renderer::reset()
{

}

int Renderer::get_render_order()
{
	auto& vec = SceneMgr::get_instance().get_render_setting()->render_order_map[render_layer];
	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i] == gameObject)
		{
			return i;
		}
	}
	Debug::logError()<<"Renderer::get_render_order() dont match renderer in vec\n";
}


//void Renderer::set_render_order(int order)
//{
//	auto& map = SceneMgr::get_instance().get_current_scene()->getAllGameObjsByDepth();
//	if (auto it = map.find(order); it != map.end())
//	{
//		auto tempObj = it->second;
//		it->second = gameObject;
//		map[render_order] = tempObj;
//		tempObj->getComponent<Renderer>()->set_render_order_directly(render_order);
//		render_order = order;
//	}
//	else
//	{
//		map.erase(render_order);
//		render_order = order;
//		map.emplace(order, gameObject);
//	}
//	onPropertyChange(properties["render_order"]);
//}

int Renderer::get_render_layer()
{
	return render_layer;
}

void Renderer::set_render_layer(int layer)
{
	SceneMgr::get_instance().unregisterRenderer(this);
	render_layer = layer;
	SceneMgr::get_instance().registerRenderer(this);
}

//void Renderer::set_render_order_directly(int order)
//{
//	render_order = order;
//}

void Renderer::serialize(PHString& str)
{
	str.appendLine(to_string((int)componentType));
	str.appendLine(to_string(render_layer));
}

void Renderer::deserialize(std::stringstream& ss)
{
	string line;
	getline(ss, line);
	set_render_layer(stoi(line));
}
