#include "IScriptBehaviour.h"
#include "Core/ResourceManagement/SceneMgr.h"

IScriptBehaviour::IScriptBehaviour(GameObject* obj):gameobj(obj)
{
	if (obj == nullptr)
		return;
	SceneMgr::get_instance().script_behaviours.push_back(this);
}

IScriptBehaviour::~IScriptBehaviour()
{
	if (gameobj == nullptr)
		return;
	auto it = std::find(SceneMgr::get_instance().script_behaviours.begin(), SceneMgr::get_instance().script_behaviours.end(), this);
	if (it != SceneMgr::get_instance().script_behaviours.end())
	{
		SceneMgr::get_instance().script_behaviours.erase(it);
	}
}
