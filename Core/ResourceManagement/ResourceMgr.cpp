#include "ResourceMgr.h"
#include "Core/SystemStatus/GameEngine.h"
#include "Core/FileIO/IO.h"
#include "SceneMgr.h"
#include "Audio.h"
#include "Core/Utils/Utils.h"

ResourceMgr* ResourceMgr::instance = nullptr;


ResourceMgr& ResourceMgr::get_instance()
{
	if (instance == nullptr)
		instance = new ResourceMgr();
	return *instance;
}

Result<void*> ResourceMgr::initialize()
{
	assetTypes.emplace(PHAsset::TEXTURE2D, new Texture2D());
	assetTypes.emplace(PHAsset::SPINE_ANIMATION, new SpineAnimationData());
	assetTypes.emplace(PHAsset::SCRIPT, new ScriptData());
	assetTypes.emplace(PHAsset::AUDIO, new Audio());
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
				texture->registerAssetToMgr();
				Debug::logInfo() << "Texture2D loaded: " << texture->get_name() << "\n";
			}				
		}
		else if(type ==".atlas")
		{
			auto spineData = loadFromPath<SpineAnimationData>(filePath,false);
			if (spineData != nullptr)
			{
				spineData->registerAssetToMgr();
				Debug::logInfo() << "SpineAnimationData loaded: " << spineData->get_name() << "\n";
			}
		}
		else if (type == ".lua")
		{
			auto scriptData = loadFromPath<ScriptData>(filePath,false);
			if (scriptData != nullptr)
			{
				scriptData->registerAssetToMgr();
				Debug::logInfo() << "ScriptData loaded: " << scriptData->get_name() << "\n";
			}
		}
		else if (contains(SUPPORT_EXTENSIONS, 4, type))
		{
			auto audio = loadFromPath<Audio>(filePath,false);
			if (audio != nullptr)
			{
				audio->registerAssetToMgr();
				Debug::logInfo() << "Audio loaded: " << audio->get_name() << "\n";
			}
		}
	}
}

void ResourceMgr::clear()
{
	for (auto& pair : assets)
	{

	}
}

Texture2D* ResourceMgr::CreatNewTexture2D(const std::string name, const std::string path)
{
	if(name!="")
	if (isExist<Texture2D>(name))
	{
		Debug::logWarning() << "Texture2D with name " << name << " already exist\n";
		return nullptr;
	}
	Texture2D* texture = new Texture2D();
	texture->name = name ;
	texture->set_img_path(path);
	return texture;
}

bool ResourceMgr::isExist(PHAsset::AssetType assetType, const std::string& name) const
{
	if(assetTypes.find(assetType)!=assetTypes.end())
		return assetTypes.at(assetType)->isExist(name);
	return false;
}

