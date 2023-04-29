#include "Text.h"
using namespace std;

Text::Text(GameObject* gameObj, const std::string& text):IBoxResizable(gameObj),text(text)
{
	componentType = ComponentType::TEXT;	
	properties.emplace("text", new Property("text", &(this->text), Property::STRING, this));
	set_size(Vector2D(380, 150));
	color = Color32("255,255,255,255");
}

void Text::set_property(Property* property, void* value)
{
	IBoxResizable::set_property(property, value);
	if (property->get_name() == "text")
	{
		set_text(*(string*)value);
	}
}

void Text::reset()
{
	IBoxResizable::reset();
	set_text("");
}

const std::string& Text::get_text()
{
	return text;
}

void Text::set_text(const std::string& text)
{
	this->text = text;
	onPropertyChanged(properties["text"]);
}

void Text::serialize(PHString& str)
{
	str.appendLine(to_string((int)componentType));
	IBoxResizable::serialize(str);
	str.appendLine(text);
}

void Text::deserialize(std::stringstream& ss)
{
	IBoxResizable::deserialize(ss);
	string line;
	getline(ss, line);
	text = line;
}
