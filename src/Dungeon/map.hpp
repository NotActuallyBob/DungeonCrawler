#pragma once
#ifndef _MAP_
#define _MAP_
#include "Actors/player.hpp"
#include "Dungeon/specialrooms/BossRoom.hpp"
#include "Dungeon/specialrooms/TreasureRoom.hpp"
#include "Dungeon/specialrooms/startingRoom.hpp"
#include "PCH.hpp"
#include "roomInstance.hpp"

class Map {

public:
    /**
     * @brief Initializes variables for new room and calls CreateDungeon()
     *
     * @param    sizeOfRooms           the size we want our rooms to be
     * @param    noRooms              The number of rooms we want the dungeon to consist of
     */
    Map(sf::Vector2u sizeOfRooms, int noRooms, PlayerPS player);

    ~Map();

    /**
     * @brief Renders the current room to the given target
     *
     */
    void RenderCurrentRoom(sf::RenderTarget* target);

    /**
     * @brief Create the actual dungeon
     *
     * @param    noRooms              number of rooms on in the dungeon
     */
    void CreateDungeon(int noRooms);

    /**
     * @brief Moves the player to the room on the map in the given direction
     *
     * @param    dir                  Direction to go
     */
    void MovePlayer(Direction dir);

    /**
     * @brief Get the Room in the argument direction
     *
     * @param    direction                 The direction
     * @return  returns room, or nullptr if not found
     */
    RoomInstance* GetRoomInDir(Direction direction);

    /**
     * @brief Converts a Direction to a unit vector and returns it
     *
     * @param    direction            the direction
     * @return sf::Vector2i the converted vector
     */
    sf::Vector2i DirToVec(Direction direction);

    /**
     * @brief Get the room the player is in on the map
     *
     * @return RoomInstance* The room
     */
    RoomInstance* GetCurrentRoom();

    /**
     * @brief Get the SpawnRoom
     *
     * @return RoomInstance* The SpawnRoom
     */
    RoomInstance* GetSpawnRoom();

    void ResetMap();

    /**
     * @brief Checks Whether the bossroom has been cleared
     *
     * @return true
     * @return false
     */
    bool IsBossRoomCleared();

private:
    std::pair<int, int> findBossRoom(std::map<std::pair<int, int>, std::set<Direction>> coordsAndWalls);
    void Move(Direction dir);

    /**
     * @brief Get the room at the wanted coordinate on the map
     *
     * @param    coord               Desciption
     * @return RoomInstance*
     */
    RoomInstance* GetRoomAt(sf::Vector2i coord);

    /**
     * @brief Gets the key for the dungeon-map for the current position on the map
     *
     * @return std::pair<int, int>
     */
    std::pair<int, int> getKey();

    /**
     * @brief Gets the key for the dungeon-map for the current position at a specific point
     *
     * @return std::pair<int, int>
     */
    std::pair<int, int> getKey(sf::Vector2i coord);
    RoomInstance* getRandomRoom();
    RoomInstance* addRoomToDungeon(sf::Vector2u roomSize, sf::Vector2i coords);
    void addStartingRoomToDungeon(sf::Vector2u roomSize, sf::Vector2i coords);
    sf::Vector2u roomSize_;
    sf::Vector2i currentPos_;
    PlayerPS player_;
    sf::Vector2i spawnCoords_;
    sf::Vector2i bossCoords_;
    std::map<std::pair<int, int>, RoomInstance*> dungeon_; // cant use vector2i as a key
    std::vector<std::pair<int, int>> existingRoomCoords_;
};

#endif
