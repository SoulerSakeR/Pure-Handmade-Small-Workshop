#include "Image.h"
#include "GameObject.h"
using namespace std;

Image::Image(GameObject* gameObj, const std::string& imgPath,Vector2D size):Component(gameObj)
{
	this->imgPath = imgPath;
	this->size = size;
	componentType = IMAGE;
}


void Image::serialize(PHString& str)
{
	str.appendLine(to_string((int)componentType));
	str.appendLine(imgPath);
	str.appendLine(size.tostring());
}

void Image::deserialize(std::stringstream& ss)
{
	string s;
	getline(ss, s);
	imgPath = s;
	getline(ss, s);
	size_t index = s.find(',');
	float x = stof(s.substr(0, index - 1));
	float y = stof(s.substr(index + 1, s.size() - 1));
	size = Vector2D(x, y);
}

void Image::reset()
{
	imgPath = "";
	size = Vector2D::zero();
}
