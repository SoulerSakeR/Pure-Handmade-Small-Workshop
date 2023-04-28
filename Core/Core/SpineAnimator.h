#pragma once
#include "IScriptBehaviour.h"
#include "IRenderable.h"
#include <spine/AnimationState.h>
#include <spine/Skeleton.h>
#include "Camera.h"

class SpineAnimator : public IRenderable , public IScriptBehaviour
{
public:
	typedef SpineAnimator customType;

	SpineAnimator(GameObject* gameobj);

	// getters and setters
	std::string get_spine_animation_name() const { return spine_animation_name; }
	bool set_spine_animation_name(const std::string& name);

	void render(Camera* camera);

	// inherited via IScriptBehaviour
	void awake() override;
	void afterUpdate() override;

	//inherited via IRenderable
	virtual void updateVertices() override ;
	virtual void createIndices() override ;

	//inherited via Component
	virtual void reset() noexcept override { spine_animation_name = ""; }
	virtual void serialize(PHString& str) override;
	virtual void deserialize(std::stringstream& ss) override;
	virtual void set_property(Property* property, void* value) override;

private:
	std::string spine_animation_name;
	spine::AnimationState* animation_state = nullptr;
	spine::Skeleton* skeleton = nullptr;
	Camera* camera = nullptr;
};

