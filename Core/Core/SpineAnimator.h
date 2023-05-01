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
	std::string get_spine_animation_name() const;											// 需要绑定
	bool set_spine_animation_name(const std::string& name);						// 需要绑定

	int get_animation_index() const;
	void set_animation_index(int index);

	int get_skin_index() const;
	void set_skin_index(int index);

	bool get_loop() const;
	void set_loop(bool loop);

	std::vector<std::string> getAllAnimations();                                               // 需要绑定
	std::vector<std::string> getAllSkins();                                                     // 需要绑定

	void render(Camera* camera);
	Result<void*> setAnimation(int index,bool loop);                                     // 需要绑定
	Result<void*> setAnimation(const std::string& name, bool loop);           // 需要绑定

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

private:
	std::string spine_animation_name;
	int animation_index;
	int skin_index;
	bool loop;
	spine::AnimationState* animation_state = nullptr;
	spine::Skeleton* skeleton = nullptr;
	Camera* camera = nullptr;
};

