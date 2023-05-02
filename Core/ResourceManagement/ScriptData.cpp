#include "ScriptData.h"
#include "Core/Utils/PHPath.h"
#include "Core/FileIO/IO.h"
#include "Core/Core/Debug.h"
#include "ResourceMgr.h"

std::string ScriptData::get_name() const
{
	return name;
}

std::string ScriptData::get_path() const
{
	return path;
}

ScriptData* ScriptData::loadFromPath(const std::string& absolutePath, bool copy)
{
	if (auto res = IO::readText(QString::fromStdString(absolutePath)); !res.isNull())
	{
		auto scriptData = new ScriptData();
		PHPath path(absolutePath);
		scriptData->name = path.getFileName(false);
		scriptData->path = absolutePath;
		if (auto it = ResourceMgr::get_instance().script_assets.find(scriptData->get_name());it != ResourceMgr::get_instance().script_assets.end())
		{
			Debug::logWarning() << "script file with name " << scriptData->get_name() << " already exist\n";
		}
		else
		{
			ResourceMgr::get_instance().script_assets[scriptData->get_name()] = scriptData;
		}
		return scriptData;
	}
	else
	{
		Debug::logError() << "Failed to load script file from path " << absolutePath << "\n";
		return nullptr;
	}
}

ScriptData* ScriptData::loadFromName(const std::string& name)
{
	auto& assets = ResourceMgr::get_instance().script_assets;
	if (assets.find(name) != assets.end())
	{
		return assets[name];
	}
	else
	{
		Debug::logWarning() << "Script with name " << name << " not exist\n";
		return nullptr;
	}
}

bool ScriptData::isExist(const std::string& name)
{
	return ResourceMgr::get_instance().isScriptExist(name);
}

ScriptData* ScriptData::CreateScriptData(const std::string& className)
{
	string luaName = className + ".lua";
	PHPath scriptDir = ResourceMgr::get_instance().getScriptDir();
	IO::createPathIfNotExists(scriptDir.getNewPath());
	if (!QFile::exists(scriptDir.combinePath("IScriptBehaviour.lua").getNewPath().c_str()))
	{
		IO::copy(PHPath(GameEngine::get_instance().getRootPath()).combinePath("IScriptBehaviour.lua").getNewPath().c_str(),
			scriptDir.getNewPath().c_str());
	}
	if (!QFile::exists(scriptDir.combinePath("Comment.lua").getNewPath().c_str()))
	{
		IO::copy(PHPath(GameEngine::get_instance().getRootPath()).combinePath("Comment.lua").getNewPath().c_str(),
			scriptDir.getNewPath().c_str());
	}
	string luaPath = PHPath(ResourceMgr::get_instance().getScriptDir()).combinePath(luaName).getNewPath();
	PHString str;
	str.appendLine("require(\"IScriptBehaviour\")");
	str.appendLine();
	str.appendLine("---@class ", className + " : IScriptBehaviour");
	str.appendLine("---@field public gameObject GameObject");
	str.appendLine(className, " = class(IScriptBehaviour)");
	if (IO::write(str.str(),luaPath,1))
	{
		return loadFromPath(luaPath);
	}
	else
	{
		Debug::logError() << "Failed to create script file " << luaPath << "\n";
		return nullptr;
	}

}
