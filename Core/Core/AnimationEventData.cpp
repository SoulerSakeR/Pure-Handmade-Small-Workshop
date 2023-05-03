#include "AnimationEventData.h"

AnimationEventData::AnimationEventData(SpineAnimator* animator, EventType type)
{
	this->animator = animator;
	this->type = type;
}
