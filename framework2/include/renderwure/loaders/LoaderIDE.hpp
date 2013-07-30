#pragma once
#ifndef _LOADERIDE_HPP_
#define _LOADERIDE_HPP_

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

class LoaderIDE
{
public:
	enum SectionTypes
	{
		NONE,
		OBJS,
		TOBJ,
		PEDS,
		CARS,
		HIER,
		TWODFX,
		PATH,
	};

	struct OBJS_t
	{
		uint16_t ID;
		std::string modelName;
		std::string textureName;
		size_t numClumps;
		float drawDistance[3];
		int32_t flags;
		bool LOD;
		
		enum {
			WET          = 1,       /// Render with a wet effect
			NIGHTONLY    = 1 << 1,  /// Render only during the night
			ALPHA1       = 1 << 2,  /// Alpha 
			ALPHA2       = 1 << 3,  /// Alpha
			DAYONLY      = 1 << 4,  /// Render only during the day
			INTERIOR     = 1 << 5,  /// Is part of an interior
			NOSHADOWMESH = 1 << 6,  /// Disable shadow mesh
			DONTCULL     = 1 << 7,  /// Disable culling
			NODRAWDIST   = 1 << 8,  /// Object won't be affected by draw distance
			BREAKABLE    = 1 << 9,  /// Object can be broken
			SMASHABLE    = 1 << 10, /// Object can be smashed and broken
			GRGEDOOR     = 1 << 11, /// Is a garage door (SA and IV only)
			MULTICLUMP   = 1 << 12, /// Multiclump
			WBRIGHTNESS  = 1 << 13, /// Weather PoleShd value effects brightness.
			EXPLODEONHIT = 1 << 14, /// Object explodes after being hit
		};
	};

	enum VehicleClass
	{
		IGNORE      = 0,
		NORMAL      = 1,
		POORFAMILY  = 1 << 1,
		RICHFAMILY  = 1 << 2,
		EXECUTIVE   = 1 << 3,
		WORKER      = 1 << 4,
		BIG         = 1 << 5,
		TAXI        = 1 << 6,
		MOPED       = 1 << 7,
		MOTORBIKE   = 1 << 8,
		LEISUREBOAT = 1 << 9,
		WORKERBOAT  = 1 << 10,
		BICYCLE     = 1 << 11,
		ONFOOT      = 1 << 12,
	};
	enum VehicleType {
		CAR,
		BOAT,
		TRAIN,
		PLANE,
		HELI,
	};
	struct CARS_t
	{
		uint16_t ID;
		std::string modelName;
		std::string textureName;
		VehicleType type;
		std::string handlingID;
		std::string gameName;
		VehicleClass classType;
		uint8_t frequency; // big enough int type?
		uint8_t lvl; // big enough int type?
		uint16_t comprules;
		union { // big enough int types?
			uint16_t wheelModelID; // used only when type == CAR
			int16_t modelLOD; // used only when type == PLANE
		};
		float wheelScale; // used only when type == CAR
		std::vector<glm::vec3> wheelPositions;
	};

	struct PEDS_t
	{
		uint16_t ID;
		std::string modelName;
		std::string textureName;
		std::string type;
		std::string behaviour;
		std::string animGroup;
		uint8_t driveMask;
	};

    enum PathType
    {
        PATH_PED,
        PATH_CAR
    };

    enum NodeType
    {
        EMPTY = 0, /// These are ignored
        EXTERNAL = 1, /// May join with other paths
        INTERNAL = 2 /// Internal to this path
    };

    struct PathNode
    {
        NodeType type;
        int32_t next;
        glm::vec3 position;
    };

    struct PATH_t
    {
        PathType type;
        uint16_t ID;
        std::string modelName;
        std::vector<PathNode> nodes;
    };

	// Load the IDE data into memory
	bool load(const std::string& filename);

	std::vector<OBJS_t> OBJSs;
	std::vector<CARS_t> CARSs;
	std::vector<PEDS_t> PEDSs;
    std::vector<PATH_t> PATHs;
};

#endif
