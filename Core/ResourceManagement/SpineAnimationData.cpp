#include "SpineAnimationData.h"
#include "SpineTextureLoader.h"
#include <spine/SkeletonJson.h>

using namespace spine;

SpineAnimationData::SpineAnimationData(const std::string& atlasAbsolutePath)
{
	atlas = new spine::Atlas(atlasAbsolutePath.c_str(),SpineTextureLoader::get_instance());

	// Create a SkeletonJson used for loading and set the scale
	// to make the loaded data two times as big as the original data
	SkeletonJson json(atlas);
	json.setScale(2);

	// Load the skeleton .json file into a SkeletonData
	PHPath atlas(atlasAbsolutePath);
	auto name = atlas.getFileName(false);
	this->name = name;
	name += ".json";
	name = atlas.getFileDir()+'\\' + name;
	skeleton_data = json.readSkeletonDataFile(name.c_str());

	// If loading failed, print the error and exit the app
	if (!skeleton_data) {
		Debug::logInfo()<< json.getError().buffer()<<"\n";
	}
	animation_state_data = new AnimationStateData(skeleton_data);
	animation_state_data->setDefaultMix(0.5f);
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

SpineAnimationData* SpineAnimationData::loadFromPath(const std::string& atlasAbsolutePath)
{
	return new SpineAnimationData(atlasAbsolutePath);
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


