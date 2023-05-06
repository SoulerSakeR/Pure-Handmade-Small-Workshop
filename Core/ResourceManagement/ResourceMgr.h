#pragma once
#include <string>
#include <iostream>
#include "Core/SystemStatus/GameEngine.h"
#include "Core/Utils/PHPath.h"
#include "Core/UI/TextureSelectorDialog.h"
#include "SpineAnimationData.h"
#include "ScriptData.h"
#include "PHAsset.h"

using std::string;

class ResourceMgr
{
public:
	const std::string SUPPORT_EXTENSIONS[4] = { ".wav",".mp3",".ogg",".flac"};
	static ResourceMgr& get_instance();

	Result<void*> initialize();
	std::string getAssetDir() const;
	std::string getSpineDir() const;
	std::string getScriptDir() const;
	std::string getSpriteDir() const;
	void loadAllAssets();
	void clear();

	Texture2D* CreatNewTexture2D(const std::string name, const std::string path);

	// api for lua script
	bool isExist(PHAsset::AssetType assetType,const std::string& name) const;

	template <typename T>
	T* loadFromPath(const string& path,bool isRelativePath = true)
	{
		if (has_type_member<T>::value)
		{
			std::string absolutePath;
			if (isRelativePath)
			{
				PHPath gamePath(GameEngine::get_instance().getGamePath());
				absolutePath = gamePath.combinePath(assetPath).combinePath(path).getNewPath();
			}
			else
			{
				absolutePath = path;
			}
			T* resource = T().loadFromPath(absolutePath,false);
			if(resource == nullptr)
				Debug::logWarning() << "ResourceMgr::load: Resourece load failed! : " << absolutePath << "\n";
			return resource;
		}
		Debug::logWarning() << "ResourceMgr::load: Resource type not supported! : " << typeid(T).name()<<"\n";
	}

	template <typename T>
	bool importFromPath(const std::string& absolutePath, bool copyToAssetDir, const std::string& desPath)
	{
		if (has_type_member<T>::value)
		{
			if (T().importFromPath(absolutePath, copyToAssetDir, desPath))
			{
				return true;
			}			
			Debug::logError() << " [ResourceMgr::importFromPath] : Resourece import failed! : " << absolutePath << "\n";
			return false;
		}
	}

	template <typename T>
	bool isExist(const string& name)
	{
		return T().isExist(name);
	}

	template <typename T>
	T* loadFromName(const string& name)
	{
		if (has_type_member<T>::value)
		{
			return dynamic_cast<T*>(T().loadFromName(name));
		}
		else
			Debug::logWarning() << "ResourceMgr::load: Resource type not supported! : " << typeid(T).name() << "\n";
	}

private:
	ResourceMgr() {};
	ResourceMgr(ResourceMgr&) = delete; 
	void operator=(ResourceMgr) = delete;
	const string assetPath ="\\Resources";
	const string spinePath = "\\Spine";
	const string scriptPath = "\\Scripts";
	const string spritePath = "\\Sprites";
	static ResourceMgr* instance;
	std::unordered_map<PHAsset::AssetType, std::unordered_map<std::string,PHAsset*>> assets;
	std::unordered_map<PHAsset::AssetType,PHAsset*> assetTypes;
	friend class Texture2D;
	friend class SpineAnimationData;
	friend class TextureSelectorDialog;
	friend class AnimationSelectorDialog;
	friend class ScriptData;
	friend class ScriptComboBox;
	friend class ScriptNameLineEdit;
	friend class PHAsset;
	friend class AssetSelectorDialog;
	friend class AssetWidget;
};

