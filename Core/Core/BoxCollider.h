#pragma once
#include "Collider.h"
#include <Core/Utils/Vector2D.h>
#include "IResizable.h"

class BoxCollider :  public IBoxResizable , public Collider
{
public:
	BoxCollider(GameObject* gameObj);
	virtual ~BoxCollider();

	virtual void set_trigger(bool value) override;

	// Inherited via Component
	void reset() override;

protected:
	void serialize(PHString& str) override;
	void deserialize(std::stringstream& ss) override;

};

