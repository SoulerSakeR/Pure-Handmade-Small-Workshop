#pragma once
#include <string>
#include <iostream>
#include "Core/SystemStatus/GameEngine.h"
#include "Core/Utils/PHPath.h"
#include "Core/UI/TextureSelectorDialog.h"
#include "SpineAnimationData.h"
#include "ScriptData.h"

using std::string;

class ResourceMgr
{
public:
	static ResourceMgr& get_instance();

	Result<void*> initialize();
	std::string getAssetDir() const;
	std::string getSpineDir() const;
	std::string getScriptDir() const;
	void loadAllAssets();
	void clear();

	Texture2D* CreatNewTexture2D(const std::string name, const std::string path);

	template <typename T>
	T* loadFromPath(const string& path,bool isRelativePath = true,bool copy = false)
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
			T* resource = T::loadFromPath(absolutePath,copy);
			if(resource == nullptr)
				Debug::logWarning() << "ResourceMgr::load: Resourece load failed! : " << absolutePath << "\n";
			return resource;
		}
		Debug::logWarning() << "ResourceMgr::load: Resource type not supported! : " << typeid(T).name()<<"\n";
	}

	template <typename T>
	bool isExist(const string& name)
	{
		return T::isExist(name);
	}

	bool isTextureExist(const string& name)
	{
		auto it = texture_assets.find(name);
		if (it != texture_assets.end())
			return true;
		return false;
	}

	bool isScriptExist(const string& name)
	{
		auto it = script_assets.find(name);
		if (it != script_assets.end())
			return true;
		return false;
	}

	bool isSpineAnimationExist(const string& name)
	{
		auto it = spine_assets.find(name);
		if (it != spine_assets.end())
			return true;
		return false;
	}

	template <typename T>
	T* loadFromName(const string& name)
	{
		if (has_type_member<T>::value)
		{
			return T::loadFromName(name);
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
	static ResourceMgr* instance;
	std::unordered_map<std::string,std::string> resourceNamePathMap;
	std::unordered_map<std::string,Texture2D*> texture_assets;
	std::unordered_map<std::string,SpineAnimationData*> spine_assets;
	std::unordered_map<std::string,ScriptData*> script_assets;
	std::unordered_map<std::string, void*> resources;

	friend class Texture2D;
	friend class SpineAnimationData;
	friend class TextureSelectorDialog;
	friend class AnimationSelectorDialog;
	friend class ScriptData;
	friend class ScriptComboBox;
	friend class ScriptNameLineEdit;
};

