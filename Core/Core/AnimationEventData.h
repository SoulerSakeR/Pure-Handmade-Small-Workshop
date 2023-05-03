#pragma once

class SpineAnimator;

class AnimationEventData
{
public:
	enum EventType {
		EventType_Start,
		EventType_Interrupt,
		EventType_End,
		EventType_Complete,
		EventType_Dispose,
		EventType_Event
	};
	AnimationEventData(SpineAnimator* animator, EventType type);
	SpineAnimator* animator;
	EventType type;
};

