#pragma once
#include "IResizable.h"
class Text : public IBoxResizable
{
public:
	Text(GameObject* gameObj, const std::string& text = "");

	// Inherited via Component
	void reset() override;

	// getters and setters
	const std::string& get_text();
	void set_text(const std::string& text);

private:
	// Inherited via ISerializable
	void serialize(PHString& str) override;
	void deserialize(std::stringstream& ss) override;

	std::string text;
};

