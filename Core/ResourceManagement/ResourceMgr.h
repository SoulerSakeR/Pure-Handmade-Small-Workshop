#pragma once
#include <string>
#include <iostream>

using std::string;

class ResourceMgr
{
public:
	static ResourceMgr& getInstance();
private:
	string assetPath;
	static ResourceMgr* _instance;
};

