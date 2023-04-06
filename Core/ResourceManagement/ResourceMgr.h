#pragma once
#include <string>
#include <iostream>

using std::string;

class ResourceMgr
{
public:
	static ResourceMgr& get_instance();
private:
	ResourceMgr() {};
	ResourceMgr(ResourceMgr&) = delete;             
	void operator=(ResourceMgr) = delete;
	const string assetPath ="Resources";
	static ResourceMgr* instance;
};

