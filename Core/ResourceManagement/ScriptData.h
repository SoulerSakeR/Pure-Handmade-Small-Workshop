#pragma once
#include <string>

class ScriptData
{
public:
	typedef ScriptData customType;

	// getters
	std::string get_name() const;
	std::string get_path() const;

	static ScriptData* loadFromPath(const std::string& absolutePath, bool copy = false);	
	static ScriptData* loadFromName(const std::string& name);
	static bool isExist(const std::string& name);
	static ScriptData* CreateScriptData(const std::string& className);

private:
	std::string name;
	std::string path;
};

