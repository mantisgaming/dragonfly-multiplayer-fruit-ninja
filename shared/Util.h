#pragma once

#include <string>

const std::string SPRITE_PATH = ".\\sprites";
const std::string SOUND_PATH = ".\\sounds";
const std::string MUSIC_PATH = ".\\music";

static class Util
{
private:
	static void loadSprites();
	static void loadSounds();
	static void loadMusic();

	static bool folderExists(std::string folder);

public:
	static void loadResources();
};

