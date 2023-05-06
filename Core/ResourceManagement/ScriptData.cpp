#include "ScriptData.h"
#include "Core/Utils/PHPath.h"
#include "Core/FileIO/IO.h"
#include "Core/Core/Debug.h"
#include "ResourceMgr.h"

ScriptData::ScriptData()
{
	assetType = SCRIPT;
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
		return ScriptData().loadFromPath(luaPath,false);
	}
	else
	{
		Debug::logError() << "Failed to create script file " << luaPath << "\n";
		return nullptr;
	}

}

ScriptData* ScriptData::loadFromPath(const std::string& path, bool isRelativePath)
{
	// Get absolute path
	string absolutePath;
	if (isRelativePath)
	{
		absolutePath = PHPath(ResourceMgr::get_instance().getAssetDir()).combinePath(path).getNewPath();
	}
	else
	{
		absolutePath = path;
	}

	// Check if file exist
	if (auto res = IO::readText(QString::fromStdString(absolutePath)); !res.isNull())
	{
		auto scriptData = new ScriptData();
		PHPath path(absolutePath);
		scriptData->name = path.getFileName(false);
		scriptData->path = PHPath(absolutePath).getRelativePath(ResourceMgr::get_instance().getAssetDir()).getNewPath();
		if (isExist(scriptData->get_name()))
		{
			Debug::logWarning() << " [ScriptData::loadFromPath] script file with name " << scriptData->get_name() << " already exist\n";
		}
		else
		{
			scriptData->registerAssetToMgr();
		}
		return scriptData;
	}
	else
	{
		Debug::logError() << "Failed to load script file from path " << absolutePath << "\n";
		return nullptr;
	}
}

