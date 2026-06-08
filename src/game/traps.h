#pragma once
#include "room.h"
#include "player.h"

constexpr int MAX_SPIKE_TILES  = 64;
constexpr int MAX_PLATE_TILES  = 16;
constexpr int MAX_DOOR_TILES   = 8;

constexpr int   DOOR_OPEN_FRAMES     = 14;
constexpr int   DOOR_IDLE_FRAMES     = 10;
constexpr float DOOR_FRAME_TIME      = 0.06f;
constexpr float DOOR_IDLE_FRAME_TIME = 0.10f;
constexpr int   DOOR_TILE_H          = 3;

struct SpikeTile {
    int  tx, ty;
    bool ceil   = false;
    bool active = false;
};

struct PlateTile {
    int  tx, ty;
    int  group   = 0;
    bool pressed = false;
    bool locked  = false;
    bool active  = false;
};

struct DoorTile {
    int   tx, ty;        // bottom tile; extends DOOR_TILE_H upward
    int   group;
    bool  flipX      = false;
    bool  locked     = false;
    bool  justOpened = false; // true for exactly one update tick when door locks
    int   openFrame  = 0;
    float openTimer  = 0.f;
    int   idleFrame  = 0;
    float idleTimer  = 0.f;
    bool  active     = false;
};

struct TrapSystem {
    SpikeTile spikes[MAX_SPIKE_TILES];
    PlateTile plates[MAX_PLATE_TILES];
    DoorTile  doors[MAX_DOOR_TILES];
    int spikeCount = 0;
    int plateCount = 0;
    int doorCount  = 0;
};

void initTraps(TrapSystem& t, Room& room);
void updateTraps(TrapSystem& t, Room& room, Player& p, float dt);
