#pragma once
#include "Collider.h"
#include <Core/Utils/Vector2D.h>

class BoxCollider : public Collider
{
public:
	BoxCollider(GameObject* gameObj);

	// getters and setters
	const Vector2D& get_size();
	void set_size(Vector2D newSize,bool refreshUI = true);

	// Inherited via Component
	void reset() override;
	void set_property(Property * property, void* value) override;

	// Inherited via IRenderable
	void updateVertices() override;
	void createIndices() override;

protected:
	void serialize(PHString& str) override;
	void deserialize(std::stringstream& ss) override;

	Vector2D size;
};

