#include "util.h"

#include <LogManager.h>
#include <ResourceManager.h>
#include <filesystem>
#include <Fader.h>

void Util::loadResources(void)
{
	loadSprites();
	loadSounds();
	loadMusic();
}

// search through the SPRITE_PATH for any sprite files and load them
void Util::loadSprites()
{
	LM.writeLog("INFO: loadResources(): Loading sprites...");
	if (!folderExists(SPRITE_PATH)) {
		LM.writeLog("WARNING: loadSprites(): No sprites folder found");
		return;
	}

	try {
		for (const auto& entry : std::filesystem::directory_iterator(SPRITE_PATH)) {
			if (entry.is_directory())
				continue;
			std::string label = entry.path().filename().replace_extension().string();
			if (!RM.loadSprite(entry.path().string(), label))
				LM.writeLog("INFO: loadSprites(): Sprite '%s' loaded", label.c_str());
		}
		LM.writeLog("INFO: loadResources(): Sprites loaded.");
	}
	catch (std::filesystem::filesystem_error e) {
		LM.writeLog("ERROR: Failed to read sprite(s)");
	}
}

// search through the SOUND_PATH for any sound files and load them
void Util::loadSounds()
{
	LM.writeLog("INFO: loadResources(): Loading sounds...");
	if (!folderExists(SOUND_PATH)) {
		LM.writeLog("WARNING: loadSprites(): No sounds folder found");
		return;
	}

	try {
		for (const auto& entry : std::filesystem::directory_iterator(SOUND_PATH)) {
			if (entry.is_directory())
				continue;
			RM.loadSound(entry.path().string(), entry.path().filename().replace_extension().string());
		}
		LM.writeLog("INFO: loadResources(): Sounds loaded");
	}
	catch (std::filesystem::filesystem_error e) {
		LM.writeLog("ERROR: Failed to read sound(s)");
	}
}

// search through the MUSIC_PATH for any music files and load them
void Util::loadMusic()
{
	LM.writeLog("INFO: loadResources(): Loading music...");
	if (!folderExists(SOUND_PATH)) {
		LM.writeLog("WARNING: loadSprites(): No music folder found");
		return;
	}

	try {
		for (const auto& entry : std::filesystem::directory_iterator(MUSIC_PATH)) {
			if (entry.is_directory())
				continue;
			RM.loadMusic(entry.path().string(), entry.path().filename().replace_extension().string());
		}
		LM.writeLog("INFO: loadResources(): Music loaded");
	}
	catch (std::filesystem::filesystem_error e) {
		LM.writeLog("ERROR: Failed to read music");
	}
}

// check to see if a folder exists
bool Util::folderExists(std::string folder)
{
	std::filesystem::path path = std::filesystem::path(folder);
	return std::filesystem::exists(path) && std::filesystem::is_directory(path);
}

void Util::drawTrail(df::Vector start, df::Vector end, df::Color color) {
#ifdef CLIENT
	const float size = 6;
	const int age = 5;
	const int opacity = 255;

	unsigned char r, g, b;
	df::colorToRGB(color, r, g, b);

	// Calculate step size for interpolation.
	float dist = df::distance(start, end) * 10;
	float dX = (start.getX() - end.getX()) / (dist + 1.0f);
	float dY = (start.getY() - end.getY()) / (dist + 1.0f);

	// Create Fader particles on line from p1 to p2.
	for (int i = 0; i < dist / 3; i++) {
		float x = end.getX() + dX * i * 3;
		float y = end.getY() + dY * i * 3;
		df::Fader* p_f = new df::Fader(size, age, opacity, r, g, b);
		p_f->setPosition(df::Vector(x, y));
	}
#endif
}
