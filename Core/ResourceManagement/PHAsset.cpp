#include "PHAsset.h"
#include "Core/ResourceManagement/ResourceMgr.h"
#include "Core/FileIO/IO.h"

PHAsset::PHAsset()
{
	assetType = UNKNOWN;
	name = "";
	path = "";
}

PHAsset::~PHAsset()
{

}

std::string PHAsset::getAssetTypeName() const
{
	return assetTypeNames[assetType];
}

std::string PHAsset::get_name() const
{
	return name;
}

std::string PHAsset::get_path() const
{
	return path;
}

PHAsset* PHAsset::loadFromName(const std::string& name)
{
	auto& assets = ResourceMgr::get_instance().assets[assetType];
	if (assets.find(name) != assets.end())
	{
		return assets[name];
	}
	else
	{
		Debug::logWarning() << assetTypeNames[assetType] << " with name " << name << " not exist\n";
		return nullptr;
	}
}

bool PHAsset::importFromPath(const std::string& absolutePath, bool copyToAssetDir, const std::string& desPath)
{
	auto assetTypeName = getAssetTypeName();
	auto result = loadFromPath(absolutePath, false);
	if (result == nullptr)
	{
		Debug::logError() << " ["<< assetTypeName <<"::importFromPath] faild to load asset from : " << absolutePath << "\n";
		return false;
	}

	// Copy to target directory
	if (copyToAssetDir)
	{
		string des;
		if (desPath != "")
		{
			des = desPath;
		}
		else
		{
			des = getDefaultDir();
		}
		IO::createPathIfNotExists(des);

		// Copy file
		if (IO::copy(absolutePath.c_str(), des.c_str()))
		{
			// Update path
			PHPath path(absolutePath);
			result->path = PHPath(des).combinePath(result->name + path.getFileType()).getNewPath();
			return true;
		}
		else // copy failed
		{
			Debug::logError() << "["<< assetTypeName<<"::loadFromPath : Failed to copy file to asset directory \n";
			Debug::logError() << "From: " << absolutePath << "\n";
			Debug::logError() << "To: " << des << "\n";
			return false;
		}
	}
	return true;
}

bool PHAsset::isExist(const std::string& name)
{
	auto& assets = ResourceMgr::get_instance().assets[assetType];
	if (assets.find(name) != assets.end())
	{
		return true;
	}
	return false;
}

std::string PHAsset::getDefaultDir() const
{
	PHPath assetDir = ResourceMgr::get_instance().getAssetDir();
	auto dir = assetDir.combinePath(assetTypeDirs[assetType]);
	return dir.getNewPath();
}

void PHAsset::registerAssetToMgr()
{
	auto& assets = ResourceMgr::get_instance().assets[assetType];
	assets[name] = this;
}

void PHAsset::unregisterAssetFromMgr()
{
	auto& assets = ResourceMgr::get_instance().assets[assetType];
	assets.erase(name);
}
