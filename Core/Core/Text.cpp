#include "Text.h"
using namespace std;

Text::Text(GameObject* gameObj, const std::string& text):IBoxResizable(gameObj)
{
	componentType = ComponentType::TEXT;	
	this->text = text;
	auto text_property = new Property("Text", &this->text, Property::STRING, this);
	text_property->set_property_func<string>(&Text::get_text, &Text::set_text, this);
	properties.emplace(text_property);
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
	onPropertyChanged(properties["Text"]);
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
