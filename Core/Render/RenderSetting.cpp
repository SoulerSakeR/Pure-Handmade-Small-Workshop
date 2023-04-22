#include "RenderSetting.h"
#include "Core/Core/Renderer.h"
#include "Core/ResourceManagement/SceneMgr.h"

using namespace std;

RenderSetting::RenderSetting()
{
	render_order_map = std::map<int, std::vector<GameObject*>>();
	render_layers = std::map<int, std::string>();
}

void RenderSetting::serialize(PHString& str)
{
	str.appendLine(RENDER_SETTING_PREFIX);
	str.appendLine(RENDER_LAYER_PREFIX,to_string(render_layers.size()));
	for (auto& it : render_layers)
	{
		str.append(to_string(it.first),",", it.second);
		str.append("\n");
	}
	str.appendLine(RENDER_LAYER_POSTFIX);
}

void RenderSetting::deserialize(std::stringstream& ss)
{
	string s;
	do
	{
		getline(ss, s);
		auto index = s.find(RENDER_LAYER_PREFIX);
		if (index != string::npos)
		{
			int size = stoi(s.substr(index + RENDER_LAYER_PREFIX.size()));
			for (int i = 0;i < size;i++)
			{
				getline(ss, s);
				index = s.find(",");
				if (index != string::npos)
				{
					int order = stoi(s.substr(0, index));
					string name = s.substr(index + 1);
					addRenderLayer(order, name);
				}
			}
		}
	} while (ss.good()&&s!=RENDER_LAYER_POSTFIX);
}

void RenderSetting::registerRenderer(Renderer* renderer)
{
	int layer = renderer->get_render_layer();
	auto it = render_order_map.find(layer);
	if (it == render_order_map.end())
	{
		Debug::logError()<< "No such layer found!\n" ;
		render_order_map[0].push_back(renderer->gameObject);
		renderer->render_layer = 0;
	}
	else
	{
		it->second.push_back(renderer->gameObject);
		refreshLater();
	}
}

void RenderSetting::unregisterRenderer(Renderer* renderer)
{
	int layer = renderer->get_render_layer();
	auto it = render_order_map.find(layer);
	if (it == render_order_map.end())
	{
		Debug::logError()<< "No such layer found!\n" ;
	}
	else
	{
		auto& vec = it->second;
		auto it2 = find(vec.begin(), vec.end(), renderer->gameObject);
		if (it2 != vec.end())
		{
			vec.erase(it2);
			refreshLater();
		}
	}
}

void RenderSetting::refreshLater()
{
	refresh_later = true;
}

void RenderSetting::refreshAll()
{
	render_order_map.clear();
	for (auto& it : render_layers)
	{
		render_order_map.emplace(it.first, std::vector<GameObject*>());
	}
	auto& vec = SceneMgr::get_instance().get_current_scene()->getRootGameObjs();
	for (auto& gameObject : vec)
	{
		addRendererWithChild(gameObject);
	}
}

void RenderSetting::addRendererWithChild(GameObject* gameObj)
{
	auto renderer = gameObj->getComponent<Renderer>();
	if (renderer != nullptr)
	{
		registerRenderer(renderer);
	}
	for (auto& child : gameObj->transform->children)
	{
		addRendererWithChild(child->gameObject);
	}
}

void RenderSetting::reset2Default(int layer)
{
	if (layer == 0)
		return;
	auto it = render_order_map.find(layer);
	if (it == render_order_map.end())
	{
		Debug::logError() << "No such layer found: " << to_string(layer) << "\n";
	}
	else
	{
		for (auto gameObject : it->second)
		{
			render_order_map[0].push_back(gameObject);
			gameObject->getComponent<Renderer>()->render_layer = 0;
		}
		it->second.clear();
	}
}

const std::map<int, std::string>& RenderSetting::get_render_layers() const
{
	return render_layers;
}

RenderSetting* RenderSetting::getDefaultSetting()
{
	auto setting = new RenderSetting();
	setting->addRenderLayer(0, "Default");
	return setting;
}

bool RenderSetting::addRenderLayer(int order,const std::string& name)
{
	auto it = render_layers.find(order);
	if (it == render_layers.end())
	{
		render_layers.emplace(order, name);
		render_order_map.emplace(order, std::vector<GameObject*>());
		return true;
	}
	else
	{
		return false;
	}
}

bool RenderSetting::removeRenderLayer(int order)
{
	auto it = render_layers.find(order);
	if (it == render_layers.end())
	{
		Debug::logError() << "No such layer found: "<<to_string(order)<< "\n";
		return false;
	}
	else
	{
		reset2Default(order);
		render_layers.erase(it);
		render_order_map.erase(order);
		return true;
	}
}
