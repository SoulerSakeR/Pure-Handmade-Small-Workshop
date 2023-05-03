#pragma once
#include "IScriptBehaviour.h"
#include "IResizable.h"
#include <spine/AnimationState.h>
#include <spine/Skeleton.h>
#include "Camera.h"
#include "Core/Utils/Result.h"
#include "AnimationEventData.h"

class AnimationEventListener;
namespace sol
{
	template <bool b>
	class basic_reference;
	using reference = basic_reference<false>;
	template <typename T, bool, typename H>
	class basic_protected_function;
	using protected_function = basic_protected_function<reference, false, reference>;
}

class SpineAnimator : public IBoxResizable , public IScriptBehaviour
{
public:
	typedef SpineAnimator customType;

	SpineAnimator(GameObject* gameobj);

	// getters and setters
	std::string get_spine_animation_name() const;											// 需要绑定
	bool set_spine_animation_name(const std::string& name);						// 需要绑定

	int get_animation_index() const;
	void set_animation_index(int index);

	int get_skin_index() const;
	void set_skin_index(int index);

	bool get_loop() const;
	void set_loop(bool loop);

	bool get_flipX() const;
	void set_flipX(bool flipX);

	bool get_flipY() const;
	void set_flipY(bool flipY);

	bool is_Valid() const;

	std::vector<std::string> getAllAnimations();                                               // 需要绑定
	std::vector<std::string> getAllSkins();                                                     // 需要绑定

	Result<void*> setAnimation(int index,bool loop);                                     // 需要绑定
	Result<void*> setAnimation(const std::string& name, bool loop);           // 需要绑定

	void addListener(sol::protected_function* callback);                                  // 需要绑定

	// inherited via IScriptBehaviour
	void awake() override;
	void afterUpdate() override;

	// inherited via IBoxResizable
	virtual void updateBorderVertices() override;

	//inherited via IRenderable
	virtual void updateVertices() override ;
	virtual void createIndices() override ;

	//inherited via Component
	virtual void reset() override;
	virtual void serialize(PHString& str) override;
	virtual void deserialize(std::stringstream& ss) override;

	PHEvent<void,AnimationEventData> onAnimationEvent;

private:
	std::string spine_animation_name;
	int animation_index;
	int skin_index;
	bool loop;
	bool flipX;
	bool flipY;
	spine::AnimationState* animation_state = nullptr;
	spine::Skeleton* skeleton = nullptr;
	AnimationEventListener* listener = nullptr;
};

