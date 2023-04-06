#include "ResourceMgr.h"
#include "Core/SystemStatus/GameEngine.h"

ResourceMgr* ResourceMgr::instance = nullptr;


ResourceMgr& ResourceMgr::get_instance()
{
	if (instance == nullptr)
		instance = new ResourceMgr();
	return *instance;
}

