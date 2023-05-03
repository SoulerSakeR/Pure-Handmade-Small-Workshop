#include "AnimationEventListener.h"
#include "lib/sol/sol.hpp"
#include "SpineAnimator.h"
#include "AnimationEventData.h"

using namespace std;
using namespace spine;

AnimationEventListener::AnimationEventListener(SpineAnimator* animator)
{
	this->animator = animator;
}

void AnimationEventListener::addListener(sol::protected_function* callback)
{
	callbacks.push_back(callback);
}

void AnimationEventListener::callback(AnimationState* state, EventType type, TrackEntry* entry, Event* event)
{
	AnimationEventData data(animator,(AnimationEventData::EventType)type);
	switch (type)
	{
	case spine::EventType_Start:
		break;
	case spine::EventType_Interrupt:
		break;
	case spine::EventType_End:
		break;
	case spine::EventType_Complete:
		break;
	case spine::EventType_Dispose:
		break;
	case spine::EventType_Event:
		break;
	default:
		break;
	}
	animator->onAnimationEvent(data);
	for (auto callback : callbacks)
	{
		sol::protected_function_result result = (*callback)(data);
		if (!result.valid())
		{
			sol::error err = result;
			std::cout << "error: " << err.what() << std::endl;
		}
	}
}
