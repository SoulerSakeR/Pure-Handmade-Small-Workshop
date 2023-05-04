#pragma once
#include <string>
#include "PHAsset.h"

class ScriptData : public PHAsset
{
public:
	ScriptData();

	static ScriptData* CreateScriptData(const std::string& className);

	// inherited via PHAsset
	virtual ScriptData* loadFromPath(const std::string& path, bool isRelativePath) override;
};

