#pragma once
#include <string>

class PHPath
{
public:
	PHPath(std::string path)
	{
		oldPath = path;
		size_t pos = 0;
		while ((pos = path.find("/", pos)) != std::string::npos) {
			path.replace(pos, 1, "\\");
			pos += 2;
		}
		newPath = path;

		pos = newPath.find_last_of(".");
		if (pos == std::string::npos) {
			// 未找到.，说明是文件夹的路径而非文件
			isFile =  false;
		}
		else {
			isFile = true;
		}
	}
	PHPath combinePath(const std::string& path);
	std::string getFileType();
	std::string getFileName(bool withExtension = true);
	std::string getNewPath();
	std::string getOldPath();
	bool getIsFile();

private:

	std::string newPath;
	std::string oldPath;
	bool isFile;
};