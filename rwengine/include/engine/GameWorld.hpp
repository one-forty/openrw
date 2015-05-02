#pragma once
#ifndef _GAMEWORLD_HPP_
#define _GAMEWORLD_HPP_

class Logger;

#include <GL/glew.h>

class GameData;
class GameState;

#include <ai/AIGraphNode.hpp>
#include <ai/AIGraph.hpp>
#include <audio/SoundManager.hpp>

class CutsceneObject;
class WorkContext;
#include <objects/ObjectTypes.hpp>

class GameObject;
class CharacterObject;
class InstanceObject;
class VehicleObject;

#include <render/VisualFX.hpp>
#include <data/ObjectData.hpp>

struct WeaponScan;

#include <glm/glm.hpp>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include <vector>
#include <set>
#include <random>
#include <array>

/**
 * Information about "Goal" locations so they can be rendered
 * (this doesn't really belong here).
 */
struct AreaIndicatorInfo
{
	enum AreaIndicatorType
	{
		Cylinder
	};
	
	AreaIndicatorType type;
	glm::vec3 position;
	glm::vec3 radius;
};


/**
 * @brief Handles all data relating to object instances and other "worldly" state.
 */
class GameWorld
{
public:

	GameWorld(Logger* log, WorkContext* work, GameData* dat);

	~GameWorld();

	Logger* logger;

	/**
	 * Loads an IPL into the game.
	 * @param name The name of the IPL as it appears in the games' gta.dat
	 */
	bool placeItems(const std::string& name);
	
	void createTraffic(const glm::vec3& near);
	void cleanupTraffic(const glm::vec3& focus);
	
	/**
	 * Creates an instance
	 */
	InstanceObject *createInstance(const uint16_t id, const glm::vec3& pos, const glm::quat& rot = glm::quat());

	/**
	 * @brief Creates an InstanceObject for use in the current Cutscene.
	 */
	CutsceneObject *createCutsceneObject(const uint16_t id, const glm::vec3& pos, const glm::quat& rot = glm::quat());
	
	/**
	 * Creates a vehicle
	 */
	VehicleObject *createVehicle(const uint16_t id, const glm::vec3& pos, const glm::quat& rot = glm::quat());

	/**
	 * Creates a pedestrian.
	 */
	CharacterObject* createPedestrian(const uint16_t id, const glm::vec3& pos, const glm::quat& rot = glm::quat());

	/**
	 * Inserts the given game object into the world.
	 */
	void insertObject(GameObject* object);

	/**
	 * Finds the GameObject with the given ID, if not found then nullptr.
	 */
	GameObject* findObject(GameObjectID id) const;

	/**
	 * Destroys an existing Object
	 */
	void destroyObject(GameObject* object);

	/**
	 * @brief Put an object on the deletion queue.
	 */
	void destroyObjectQueued(GameObject* object);

	/**
	 * @brief Destroys all objects on the destruction queue.
	 */
	void destroyQueuedObjects();

	/**
	 * Performs a weapon scan against things in the world
	 */
	void doWeaponScan(const WeaponScan &scan );

	/**
	 * Allocates a new VisualFX of the given type
	 */
	VisualFX* createEffect(VisualFX::EffectType type);
	
	/**
	 * Immediately destoys the given effect
	 */
	void destroyEffect(VisualFX* effect);

	/**
	 * Returns the current hour
	 */
	int getHour();
	
	/**
	 * Returns the current minute
	 */
	int getMinute();

	glm::vec3 getGroundAtPosition(const glm::vec3& pos) const;
	
	/** 
	 * Game Clock
	 */
	float gameTime;
	
	/**
	 * Game data
	 */
	GameData* data;

	/**
	 * Gameplay state
	 */
	GameState* state;
	
	/**
	 * State of playing sounds
	 */
	SoundManager sound;

	/**
	 * The active GameObjects within the world, mapped to their allocated ID
	 */
	std::map<GameObjectID, GameObject*> objects;

	std::set<GameObject*> characters;

	/**
	 * Stores objects within a grid cell, and their maximum
	 * bounding radius
	 */
	struct GridCell
	{
		/**
		 * Static instances within this grid cell
		 */
		std::set<GameObject*> instances;
		float boundingRadius = 0.f;
	};
	std::array<GridCell, WORLD_GRID_CELLS> worldGrid;

	/**
	 * returns true if the given object should be stored
	 * within the grid
	 */
	bool shouldBeOnGrid(GameObject* object);
	void addToGrid(GameObject* object);

	/**
	 * Returns the grid coordinates for a world coordinates
	 */
	glm::ivec2 worldToGrid(const glm::vec2& world);

	/**
	 * Map of Model Names to Instances
	 */
	std::map<std::string, InstanceObject*> modelInstances;

	/**
	 * AI Graph
	 */
	AIGraph aigraph;
	
	/**
	 * Visual Effects
	 * @todo Consider using lighter handing mechanism
	 */
	std::vector<VisualFX*> effects;

	/**
	 * Randomness Engine
	 */
	std::default_random_engine randomEngine;
	
	/**
	 * Bullet 
	 */
	btDefaultCollisionConfiguration* collisionConfig;
	btCollisionDispatcher* collisionDispatcher;
	btBroadphaseInterface* broadphase;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	/**
	 * @brief physicsNearCallback
	 * Used to implement uprooting and other physics oddities.
	 */
	static bool ContactProcessedCallback(btManifoldPoint& mp, void* body0, void* body1);

	/**
	 * @brief PhysicsTickCallback updates object each physics tick.
	 * @param physWorld
	 * @param timeStep
	 */
	static void PhysicsTickCallback(btDynamicsWorld* physWorld, btScalar timeStep);

	/**
	 * Work related
	 */
	WorkContext* _work;

	/**
	 * @brief Loads and starts the named cutscene.
	 * @param name
	 */
	void loadCutscene(const std::string& name);
	void startCutscene();
	void clearCutscene();
	bool isCutsceneDone();

	sf::SoundStream* cutsceneAudio;
	bool cutsceneAudioLoaded;
	sf::SoundBuffer* missionAudio;
	sf::Sound missionSound;

	/**
	 * @brief loads a model into a special character slot.
	 */
	void loadSpecialCharacter(const unsigned short index, const std::string& name);
	void loadSpecialModel(const unsigned short index, const std::string& name);
	
	void disableAIPaths(AIGraphNode::NodeType type, const glm::vec3& min, const glm::vec3& max);
	void enableAIPaths(AIGraphNode::NodeType type, const glm::vec3& min, const glm::vec3& max);
	
	void drawAreaIndicator(AreaIndicatorInfo::AreaIndicatorType type, glm::vec3 position, glm::vec3 radius);
	
	const std::vector<AreaIndicatorInfo>& getAreaIndicators() const { return areaIndicators; }
	
	void clearTickData();
	
	void setPaused(bool pause);
	bool isPaused() const;

private:

	/**
	 * @brief Used by objects to delete themselves during updates.
	 */
	std::set<GameObject*> deletionQueue;

	std::vector<AreaIndicatorInfo> areaIndicators;
	
	/**
	 * Flag for pausing the simulation
	 */
	bool paused;
};

#endif
