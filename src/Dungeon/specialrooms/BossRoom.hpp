#ifndef _BOSS_ROOM_
#define _BOSS_ROOM_

#include "Actors/Monsters/MonsterSpawner/BossSpawner.hpp"
#include "Dungeon/roomInstance.hpp"

/**
 * @brief The roominstance where the boss spawns and which has to be cleared to beat the game
 *
 */
class BossRoom : public RoomInstance {
public:

    /**
     * @brief \see RoomInstance::RoomInstance(sf::Vector2u window_size, sf::Vector2i coords)
     *
     */
    BossRoom(sf::Vector2u window_size, sf::Vector2i coords);
    BossRoom() { }
    ~BossRoom() { }
    /**
     * @brief
     *
     * @param    player               Desciption
     * @param    direction            Desciption
     */
    virtual void Enter(PlayerPS player, Direction direction);
    virtual void setTiles(sf::Vector2u window_size);
};

#endif