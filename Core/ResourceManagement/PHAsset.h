#pragma once
#include <string>
#include "Core/Utils/Macro.h"
#include "Core/Utils/PHPath.h"

class PHAsset
{
public:
	typedef PHAsset customType;
	enum AssetType
	{
		UNKNOWN ,TEXTURE2D , SCRIPT, SPINE_ANIMATION, AUDIO
	};
	const std::string assetTypeNames[5] = { "UNKNOWN" ,"TEXTURE2D" , "SCRIPT", "SPINE_ANIMATION", "AUDIO" };
	const std::string assetTypeDirs[5] = { "","Texture2D" , "Script", "SpineAnimation", "Audio" };

	PHAsset();
	virtual ~PHAsset();

	std::string getAssetTypeName() const; 

	// getters
	std::string get_name() const;
	std::string get_path() const;

	// Interface of Asset
	virtual PHAsset* loadFromName(const std::string& name);
	virtual PHAsset* loadFromPath(const std::string& path,bool isRelativePath) = 0;
	virtual bool importFromPath(const std::string& absolutePath, bool copyToAssetDir = true, const std::string& desPath = "");
	virtual bool isExist(const std::string& name);
	std::string getDefaultDir() const;
	void registerAssetToMgr();
	void unregisterAssetFromMgr();

	AssetType assetType;
protected:	
	std::string name;
	std::string path;
private:
	NO_COPY_AND_ASSIGN(PHAsset);
};

