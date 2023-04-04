#include "ResourceMgr.h"

ResourceMgr* ResourceMgr::_instance = nullptr;

ResourceMgr& ResourceMgr::getInstance()
{
	if (_instance == nullptr)
		_instance = new ResourceMgr();
	return *_instance;
}
