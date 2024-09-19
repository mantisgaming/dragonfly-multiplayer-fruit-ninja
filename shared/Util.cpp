#include "util.h"

#include <LogManager.h>
#include <ResourceManager.h>
#include <filesystem>

void Util::loadResources(void)
{
	loadSprites();
	loadSounds();
	loadMusic();
}

// search through the SPRITE_PATH for any sprite files and load them
void Util::loadSprites()
{
	LM.writeLog("loadResources(): INFO: Loading sprites...");
	if (!folderExists(SPRITE_PATH)) {
		LM.writeLog("loadSprites(): WARNING: No sprites folder found");
		return;
	}

	try {
		for (const auto& entry : std::filesystem::directory_iterator(SPRITE_PATH)) {
			if (entry.is_directory())
				continue;
			RM.loadSprite(entry.path().string(), entry.path().filename().replace_extension().string());
		}
		LM.writeLog("loadResources(): INFO: Sprites loaded.");
	}
	catch (std::filesystem::filesystem_error e) {
		LM.writeLog("ERROR: Failed to read sprite(s)");
	}
}

// search through the SOUND_PATH for any sound files and load them
void Util::loadSounds()
{
	LM.writeLog("loadResources(): INFO: Loading sounds...");
	if (!folderExists(SOUND_PATH)) {
		LM.writeLog("loadSprites(): WARNING: No sounds folder found");
		return;
	}

	try {
		for (const auto& entry : std::filesystem::directory_iterator(SOUND_PATH)) {
			if (entry.is_directory())
				continue;
			RM.loadSound(entry.path().string(), entry.path().filename().replace_extension().string());
		}
		LM.writeLog("loadResources(): INFO: Sounds loaded");
	}
	catch (std::filesystem::filesystem_error e) {
		LM.writeLog("ERROR: Failed to read sound(s)");
	}
}

// search through the MUSIC_PATH for any music files and load them
void Util::loadMusic()
{
	LM.writeLog("loadResources(): INFO: Loading music...");
	if (!folderExists(SOUND_PATH)) {
		LM.writeLog("loadSprites(): WARNING: No music folder found");
		return;
	}

	try {
		for (const auto& entry : std::filesystem::directory_iterator(MUSIC_PATH)) {
			if (entry.is_directory())
				continue;
			RM.loadMusic(entry.path().string(), entry.path().filename().replace_extension().string());
		}
		LM.writeLog("loadResources(): INFO: Music loaded");
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
