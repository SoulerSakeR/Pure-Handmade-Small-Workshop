#include "ResourceMgr.h"
#include "Core/SystemStatus/GameEngine.h"
#include "Core/FileIO/IO.h"
#include "SceneMgr.h"

ResourceMgr* ResourceMgr::instance = nullptr;


ResourceMgr& ResourceMgr::get_instance()
{
	if (instance == nullptr)
		instance = new ResourceMgr();
	return *instance;
}

Result<void*> ResourceMgr::initialize()
{
	return Result<void*>();
}

std::string ResourceMgr::getAssetDir() const
{
	return PHPath(GameEngine::get_instance().getGamePath()).combinePath(assetPath).getNewPath();
}

std::string ResourceMgr::getSpineDir() const
{
	return PHPath(GameEngine::get_instance().getGamePath()).combinePath(assetPath).combinePath(spinePath).getNewPath();
}

std::string ResourceMgr::getScriptDir() const
{
	return PHPath(GameEngine::get_instance().getGamePath()).combinePath(assetPath).combinePath(scriptPath).getNewPath();
}

void ResourceMgr::loadAllAssets()
{
	clear();
	PHPath gamePath(GameEngine::get_instance().getGamePath());
	PHPath absolutePath = gamePath.combinePath(assetPath);
	auto files = IO::getFilesInDirectory(absolutePath.getNewPath(),true);
	for (auto& filePath : files)
	{
		auto type = PHPath(filePath).getFileType();
		if (type == ".texture")
		{
			auto texture = loadFromPath<Texture2D>(filePath,false);
			if (texture != nullptr)
			{
				texture_assets[texture->get_name()] = texture;
				Debug::logInfo() << "Texture2D loaded: " << texture->get_name() << "\n";
			}				
		}
		else if(type ==".atlas")
		{
			auto spineData = loadFromPath<SpineAnimationData>(filePath,false);
			if (spineData != nullptr)
			{
				spine_assets[spineData->get_name()] = spineData;
				Debug::logInfo() << "SpineAnimationData loaded: " << spineData->get_name() << "\n";
			}
		}
		else if (type == ".lua")
		{
			auto scriptData = loadFromPath<ScriptData>(filePath,false);
			if (scriptData != nullptr)
			{
				script_assets[scriptData->get_name()] = scriptData;
				Debug::logInfo() << "ScriptData loaded: " << scriptData->get_name() << "\n";
			}
		}
	}
}

void ResourceMgr::clear()
{
	auto vec = std::vector<Texture2D*>(texture_assets.size());
	for (auto& texture : texture_assets)
	{
		vec.push_back(texture.second);
	}
	for (auto& texture : vec)
	{
		delete texture;
	}
	texture_assets.clear();
	auto vec2 = std::vector<SpineAnimationData*>(spine_assets.size());
	for (auto& spine : spine_assets)
	{
		vec2.push_back(spine.second);
	}
	for (auto& spine : vec2)
	{
		delete spine;
	}
	spine_assets.clear();
}

Texture2D* ResourceMgr::CreatNewTexture2D(const std::string name, const std::string path)
{
	if(name!="")
	if (texture_assets.find(name) != texture_assets.end())
	{
		Debug::logWarning() << "Texture2D with name " << name << " already exist\n";
		return nullptr;
	}
	Texture2D* texture = new Texture2D();
	texture->name = name ;
	texture->set_img_path(path);
	return texture;
}

