#pragma once
#include "IScriptBehaviour.h"
#include "IRenderable.h"
#include <spine/AnimationState.h>
#include <spine/Skeleton.h>
#include "Camera.h"
#include "Core/Utils/Result.h"

class SpineAnimator : public IRenderable , public IScriptBehaviour
{
public:
	typedef SpineAnimator customType;

	SpineAnimator(GameObject* gameobj);

	// getters and setters
	std::string get_spine_animation_name() const { return spine_animation_name; }
	bool set_spine_animation_name(const std::string& name);

	std::vector<std::string> getAllAnimations();

	void render(Camera* camera);
	Result<void*> setAnimation(int index,bool loop);
	Result<void*> setAnimation(const std::string& name, bool loop);

	// inherited via IScriptBehaviour
	void awake() override;
	void afterUpdate() override;

	//inherited via IRenderable
	virtual void updateVertices() override ;
	virtual void createIndices() override ;

	//inherited via Component
	virtual void reset() override;
	virtual void serialize(PHString& str) override;
	virtual void deserialize(std::stringstream& ss) override;
	virtual void set_property(Property* property, void* value) override;

private:
	std::string spine_animation_name;
	int animation_index = 0;
	spine::AnimationState* animation_state = nullptr;
	spine::Skeleton* skeleton = nullptr;
	Camera* camera = nullptr;
};

