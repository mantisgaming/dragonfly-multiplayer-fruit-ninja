#pragma once

#include <string>
#include <Vector.h>
#include <Color.h>

const std::string SPRITE_PATH = ".\\resources\\sprites";
const std::string SOUND_PATH = ".\\resources\\sounds";
const std::string MUSIC_PATH = ".\\resources\\music";

class Util
{
private:
	static void loadSprites();
	static void loadSounds();
	static void loadMusic();

	static bool folderExists(std::string folder);

public:
	static void loadResources();
	static void drawTrail(df::Vector start, df::Vector end, df::Color color);
};

