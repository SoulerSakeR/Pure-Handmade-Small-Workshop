#pragma once
#include <vector>
#include <spine/AnimationState.h>

class SpineAnimator;
namespace sol
{
	template <bool b>
	class basic_reference;
	using reference = basic_reference<false>;
	template <typename T, bool, typename H>
	class basic_protected_function;
	using protected_function = basic_protected_function<reference, false, reference>;
}

class AnimationEventListener : public spine::AnimationStateListenerObject
{
public:
	AnimationEventListener(SpineAnimator* animator);

	void addListener(sol::protected_function* callback);
	virtual void callback(spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* event) override;
private:
	SpineAnimator* animator;
	std::vector<sol::protected_function*> callbacks;
};

