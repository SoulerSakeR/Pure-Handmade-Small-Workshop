#pragma once
#include "Collider.h"
#include <Core/Utils/Vector2D.h>
#include "IResizable.h"

class BoxCollider :  public IBoxResizable , public Collider
{
public:
	BoxCollider(GameObject* gameObj);
	virtual ~BoxCollider();

	// Inherited via Component
	void reset() override;
	void set_property(Property * property, void* value) override;

protected:
	void serialize(PHString& str) override;
	void deserialize(std::stringstream& ss) override;

};

