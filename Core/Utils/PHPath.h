#pragma once
#include <string>

class PHPath
{
public:
	PHPath(const std::string& path)
	{
		std::string temp = path;
		oldPath = path;
		size_t pos = 0;
		while ((pos = path.find("/", pos)) != std::string::npos) {
			temp.replace(pos, 1, "\\");
			pos += 2;
		}
		newPath = temp;

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
	std::string getFileDir(); //获取文件目录，如果文件在根目录下则返回根目录
	bool getIsFile();

private:

	std::string newPath;
	std::string oldPath;
	bool isFile;
};