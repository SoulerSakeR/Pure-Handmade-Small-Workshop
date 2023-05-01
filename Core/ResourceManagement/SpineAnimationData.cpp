#include "SpineAnimationData.h"
#include "SpineTextureLoader.h"
#include <spine/SkeletonJson.h>
#include "Core/FileIO/IO.h"

using namespace spine;

SpineAnimationData::SpineAnimationData(const std::string& atlasAbsolutePath)
{
	name = "";
}

std::string SpineAnimationData::getTexturePath() const
{
	auto object = atlas->getPages()[0]->getRendererObject();
	if (object != nullptr)
	{
		auto texture2D = static_cast<Texture2D*>(object);
		return texture2D->get_img_path();
	}
	return "";
}

SpineAnimationData* SpineAnimationData::loadFromPath(const std::string& atlasAbsolutePath, bool copy)
{
	auto result = new SpineAnimationData(atlasAbsolutePath);
	result->atlas = new spine::Atlas(atlasAbsolutePath.c_str(), SpineTextureLoader::get_instance());

	// Create a SkeletonJson used for loading and set the scale
	// to make the loaded data two times as big as the original data
	if (result->atlas == nullptr)
	{
		Debug::logError() << "Error reading atlas file: " << atlasAbsolutePath << "\n";
		return nullptr;
	}
	SkeletonJson json(result->atlas);
	json.setScale(2);

	// Load the skeleton .json file into a SkeletonData
	PHPath atlas(atlasAbsolutePath);
	auto name = atlas.getFileName(false);
	result->name = name;
	auto jsonName = name + ".json";
	auto textureName = name + ".png";
	auto jsonPath = atlas.getFileDir() + '\\' + jsonName;
	auto texturePath = atlas.getFileDir() + '\\' + textureName;
	result->skeleton_data = json.readSkeletonDataFile(jsonPath.c_str());

	// If loading failed, print the error and exit the app
	if (!result->skeleton_data) {
		Debug::logError() << "Error reading skeleton file: " << name << "\n";
		Debug::logInfo() << json.getError().buffer() << "\n";
		return nullptr;
	}
	result->animation_state_data = new AnimationStateData(result->skeleton_data);
	result->animation_state_data->setDefaultMix(0.5f);
	if (auto it = ResourceMgr::get_instance().spine_assets.find(result->get_name());it != ResourceMgr::get_instance().spine_assets.end())
	{
		Debug::logWarning() << "Spine animation with name " << result->get_name() << " already exist\n";
		return nullptr;
	}
	else
	{
		if (copy)
		{
			PHPath spineDir(ResourceMgr::get_instance().getSpineDir());
			spineDir = spineDir.combinePath(result->get_name());
			IO::createPathIfNotExists(spineDir.getNewPath());
			IO::copy(atlasAbsolutePath.c_str(), spineDir.getNewPath().c_str());
			IO::copy(jsonPath.c_str(), spineDir.getNewPath().c_str());
			IO::copy(texturePath.c_str(), spineDir.getNewPath().c_str());
		}
		ResourceMgr::get_instance().spine_assets[result->get_name()] = result;
		return result;
	}	
}

SpineAnimationData* SpineAnimationData::loadFromName(const std::string& name)
{
	auto& assets = ResourceMgr::get_instance().spine_assets;
	if (assets.find(name) != assets.end())
	{
		return assets[name];
	}
	else
	{
		Debug::logWarning() << "SpineAnimation with name " << name << " not exist\n";
		return nullptr;
	}
}

bool SpineAnimationData::isExist(const std::string& name)
{
	return ResourceMgr::get_instance().isSpineAnimationExist(name);
}



