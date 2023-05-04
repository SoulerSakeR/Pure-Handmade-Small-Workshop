#pragma once
#include <string>
#include <spine/Atlas.h>
#include <spine/SkeletonData.h>
#include <spine/AnimationStateData.h>
#include "PHAsset.h"

class SpineAnimationData : public PHAsset
{
public:
	SpineAnimationData();
	// TODO : virtual ~SpineAnimationData();
	
	// getters
	spine::Atlas* getAtlas() const { return atlas; }
	spine::SkeletonData* get_skeleton_data() const { return skeleton_data; }
	spine::AnimationStateData* get_animation_state_data() const { return animation_state_data; }
	std::string getTexturePath() const;

	// inherited via PHAsset
	virtual SpineAnimationData* loadFromPath(const std::string& path, bool isRelativePath);

private:
	spine::Atlas* atlas = nullptr;
	spine::SkeletonData* skeleton_data = nullptr;
	spine::AnimationStateData* animation_state_data = nullptr;
};

