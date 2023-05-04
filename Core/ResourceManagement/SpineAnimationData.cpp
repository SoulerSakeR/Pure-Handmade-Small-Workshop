#include "SpineAnimationData.h"
#include "SpineTextureLoader.h"
#include <spine/SkeletonJson.h>
#include "Core/FileIO/IO.h"

using namespace spine;

SpineAnimationData::SpineAnimationData()
{
	assetType = SPINE_ANIMATION;
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

SpineAnimationData* SpineAnimationData::loadFromPath(const std::string& path, bool isRelativePath)
{
	// Get path
	PHPath absolutePath(path);
	if (isRelativePath)
	{
		absolutePath = PHPath(ResourceMgr::get_instance().getAssetDir()).combinePath(path);
	}

	auto result = new SpineAnimationData();
	result->atlas = new spine::Atlas(absolutePath.getNewPath().c_str(), SpineTextureLoader::get_instance());

	// Create a SkeletonJson used for loading and set the scale
	// to make the loaded data two times as big as the original data
	if (result->atlas == nullptr)
	{
		Debug::logError() << " [SpineAnimationData::loadFromPath] Error reading atlas file: " << absolutePath.getNewPath() << "\n";
		return nullptr;
	}
	SkeletonJson json(result->atlas);
	json.setScale(2);

	// Load the skeleton .json file into a SkeletonData
	PHPath atlas(absolutePath);
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
	if (isExist(result->get_name()))
	{
		Debug::logWarning() << "Spine animation with name " << result->get_name() << " already exist\n";
		return nullptr;
	}
	else
	{
		result->registerAssetToMgr();
		return result;
	}
}




