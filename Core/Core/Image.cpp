#include "Image.h"


Image::Image(const std::string& imgPath) :imgPath(imgPath)
{
	componentType = IMAGE;
}

void Image::serialize(PHString&)
{
}

void Image::deserialize(std::stringstream& ss)
{
}
