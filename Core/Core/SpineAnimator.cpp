#include "SpineAnimator.h"
#include "Core/ResourceManagement/ResourceMgr.h"
#include "Core/Utils/Time.h"
#include "Property.h"

SpineAnimator::SpineAnimator(GameObject* gameobj): IRenderable(gameobj)
{
	componentType = ComponentType::SPINE_ANIMATOR;
	properties.emplace("SpineAnimationName", new Property("SpineAnimationName", &spine_animation_name, Property::STRING, this));
}

bool SpineAnimator::set_spine_animation_name(const std::string& name)
{
	auto animationdata = ResourceMgr::get_instance().loadFromName<SpineAnimationData>(name);
	if (animationdata != nullptr)
	{
		spine_animation_name = name;
		//skeleton = new spine::Skeleton(animationdata->get_skeleton_data());
		//animation_state = new spine::AnimationState(animationdata->get_animation_state_data());
		return true;
	}
	return false;
}

void SpineAnimator::awake()
{
	auto animationdata = ResourceMgr::get_instance().loadFromName<SpineAnimationData>(spine_animation_name);
	if (animationdata != nullptr)
	{
		skeleton = new spine::Skeleton(animationdata->get_skeleton_data());
		animation_state =  new spine::AnimationState(animationdata->get_animation_state_data());
	}
	else {
		Debug::logWarning() << "SpineAnimator::awake: SpineAnimationData load failed! : " << spine_animation_name << "\n";
	}
}

void SpineAnimator::afterUpdate()
{
	if (animation_state != nullptr && skeleton != nullptr)
	{
		animation_state->update(Time::deltaTime);
		animation_state->apply(*skeleton);
		skeleton->updateWorldTransform();
	}
}

void SpineAnimator::set_property(Property* property, void* value)
{
	if (property->get_name() == "SpineAnimationName")
	{
		set_spine_animation_name(*(std::string*)value);
	}
	else
	{
		IRenderable::set_property(property, value);
	}
}
