#include "Audio.h"
#include "Core/FileIO/IO.h"
#include "Core/ResourceManagement/ResourceMgr.h"

Audio::Audio()
{
    assetType = AUDIO;
    audioType = MUSIC;
}

Audio* Audio::loadFromPath(const std::string& path, bool isRelativePath)
{
    // Get path
    PHPath audioPath(path);
    if (isRelativePath)
    {
		audioPath = PHPath(ResourceMgr::get_instance().getAssetDir()).combinePath(path);
	}

    // Check if file exists
    if (!QFile::exists(audioPath.getNewPath().c_str()))
    {
        Debug::logWarning()<< "Audio::loadFromPath : File does not exist: " << audioPath.getNewPath() << "\n";
        return nullptr;
    }

    // Create new audio
    auto audio = new Audio();
    auto relativePath = audioPath.getRelativePath(ResourceMgr::get_instance().getAssetDir());
    audio->path = relativePath.getNewPath();
    audio->name = audioPath.getFileName(false);

    // Check if audio with the same name already exists
    if (isExist(audio->name))
    {
		Debug::logWarning() << "Audio::loadFromPath : Audio with name " << audio->name << " already exists\n";
		return audio;
	}

    // Check audio type
    auto type = audioPath.getFileType();
    if (type == ".wav")
    {
        audioType = SOUND_EFFECT;
    }

    audio->registerAssetToMgr();
    return audio;
}
