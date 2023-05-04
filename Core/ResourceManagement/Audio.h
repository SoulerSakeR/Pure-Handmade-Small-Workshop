#pragma once
#include "PHAsset.h"

class Audio : public PHAsset
{
public:
	enum AudioType
	{
		MUSIC, SOUND_EFFECT
	};
	Audio();
	virtual Audio* loadFromPath(const std::string& path, bool isRelativePath) override;

	AudioType audioType;
};

