#include "PHPath.h"

std::string PHPath::getNewPath() {
	return newPath;
}

std::string PHPath::getOldPath() {
	return oldPath;
}

std::string PHPath::getFileName() {

    size_t pos = newPath.find_last_of("\\");
    return newPath.substr(pos + 1);
}
bool PHPath::getIsFile() {

	return isFile;
}