#pragma once
#ifndef _GAMESTATE_HPP_
#define _GAMESTATE_HPP_
#include <glm/glm.hpp>
#include <string>
#include <map>
#include <vector>

class PlayerController;
struct CutsceneData;

struct TextDisplayData
{
	// This is set by the final display text command.
	std::string text;
	glm::vec2 position;

	glm::vec4 colourFG;
	glm::vec4 colourBG;
};

struct OnscreenText
{
	std::string osTextString;
	float osTextStart;
	float osTextTime;
	unsigned short osTextStyle;
};

/**
 * Stores information about where the game can generate vehicles.
 */
struct VehicleGenerator
{
	glm::vec3 position;
	float heading;
	/** ID of the vehicle to spawn, or -1 for random. */
	int vehicleID;
	int colourFG;
	int colourBG;
	bool alwaysSpawn;
	short alarmThreshold;
	short lockedThreshold;
	
	int minDelay;
	int maxDelay;
	/** Incrementing timer (in ms), will only spawn a vehicle when minDelay < spawnTimer and will always spawn if maxDelay < spawnTimer */
	int lastSpawnTime;
	
	/** Number of vehicles left to spawn 0-100, 101 = never decrement. */
	int remainingSpawns;
};

struct GameState
{
	unsigned int maxProgress;
	unsigned int numMissions;
	unsigned int numHiddenPackages;
	unsigned int numHiddenPackagesDiscovered;
	unsigned int numUniqueJumps;
	unsigned int numRampages;
	unsigned int maxWantedLevel;
	PlayerController* player;

	unsigned int currentWeather;

	/**
	 * @brief Stores a pointer to script global that stores the on-mission state.
	 */
	unsigned int *scriptOnMissionFlag;

	bool fadeOut;
	float fadeStart;
	float fadeTime;
	bool fadeSound;
	glm::u16vec3 fadeColour;

	std::string currentSplash;


	bool isIntroPlaying;
	CutsceneData* currentCutscene;
	float cutsceneStartTime;
	/** Flag for rendering cutscene letterbox */
	bool isCinematic;

	short hour;
	short minute;
	
	
	/// Stores the "special" character and cutscene model indices.
	std::map<unsigned short, std::string> specialCharacters;
	std::map<unsigned short, std::string> specialModels;

	std::vector<OnscreenText> text;
	
	TextDisplayData nextText;
	std::vector<TextDisplayData> texts;

	/** The camera near value currently set by the script */
	float cameraNear;
	bool cameraFixed;
	glm::vec3 cameraPosition;
	glm::quat cameraRotation;
	
	std::vector<VehicleGenerator> vehicleGenerators;

	GameState() :
		maxProgress(1),
		numMissions(0),
		numHiddenPackages(0),
		numHiddenPackagesDiscovered(0),
		numUniqueJumps(0),
		numRampages(0),
		maxWantedLevel(0),
		player(nullptr),
		currentWeather(0),
		scriptOnMissionFlag(nullptr),
		fadeOut(true),
		fadeStart(0.f),
		fadeTime(0.f),
		fadeSound(false),
		isIntroPlaying(false),
		currentCutscene(nullptr),
		cutsceneStartTime(-1.f),
		isCinematic(false),
		hour(0),
		minute(0),
		cameraNear(0.1f),
		cameraFixed(false)
	{}
};

#endif
