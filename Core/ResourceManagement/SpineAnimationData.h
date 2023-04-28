#pragma once
#include <string>
#include <spine/Atlas.h>
#include <spine/SkeletonData.h>
#include <spine/AnimationStateData.h>

class SpineAnimationData
{
public:
	SpineAnimationData(const std::string& atlasAbsolutePath);
	typedef SpineAnimationData customType;

	// getters
	spine::Atlas* getAtlas() const { return atlas; }
	spine::SkeletonData* get_skeleton_data() const { return skeleton_data; }
	spine::AnimationStateData* get_animation_state_data() const { return animation_state_data; }

	static SpineAnimationData* loadFromPath(const std::string& atlasAbsolutePath);
	static SpineAnimationData* loadFromName(const std::string& name);

	std::string get_name() const { return name; }

private:
	std::string name;
	spine::Atlas* atlas = nullptr;
	spine::SkeletonData* skeleton_data = nullptr;
	spine::AnimationStateData* animation_state_data = nullptr;
};

