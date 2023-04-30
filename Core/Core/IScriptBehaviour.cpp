#include "IScriptBehaviour.h"
#include "Core/ResourceManagement/SceneMgr.h"

IScriptBehaviour::IScriptBehaviour()
{
	SceneMgr::get_instance().script_behaviours.push_back(this);
}

IScriptBehaviour::~IScriptBehaviour()
{
	auto it = std::find(SceneMgr::get_instance().script_behaviours.begin(), SceneMgr::get_instance().script_behaviours.end(), this);
	if (it != SceneMgr::get_instance().script_behaviours.end())
	{
		SceneMgr::get_instance().script_behaviours.erase(it);
	}
}
