#include "PHPath.h"

std::string PHPath::getNewPath() {
	return newPath;
}

std::string PHPath::getOldPath() {
	return oldPath;
}

PHPath PHPath::combinePath(const std::string& path)
{
	auto temp = path;
	size_t pos = 0;
	while ((pos = temp.find("/", pos)) != std::string::npos) {
		temp.replace(pos, 1, "\\");
		pos += 2;
	}
	std::string result = newPath;
	if (temp[0] != '\\')
		result.append("\\");
	result.append(temp);
	return PHPath(result);
}

std::string PHPath::getFileType()
{
	size_t pos = newPath.find_last_of(".");
	return newPath.substr(pos);
}

std::string PHPath::getFileName(bool withExtension) {

	if (withExtension)
	{
		size_t pos = newPath.find_last_of("\\");
		return newPath.substr(pos + 1);
	}
	else
	{
		size_t pos1 = newPath.find_last_of("\\");
		size_t pos2 = newPath.find_last_of(".");
		return newPath.substr(pos1 + 1,pos2-pos1-1);
	}
    
}
bool PHPath::getIsFile() {

	return isFile;
}


std::string PHPath::getFileDir(){
	for (unsigned i = newPath.size(); i > 0; --i) {
		if (newPath[i] == '/') {
			return std::string(newPath.begin(), newPath.begin() + i);
		}
	}
	if (newPath[0] == '/') {
		return "/";
	}
	else {
		return ".";
	}
}