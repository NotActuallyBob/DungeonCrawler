#pragma once

#ifndef _SLOW_MONSTER_CLASS_
#define _SLOW_MONSTER_CLASS_

#include "Actors/character.hpp"
#include "Actors/player.hpp"
#include "Combat/Weapons/BowWeapon.hpp"
#include "monster.hpp"

/**
 * @brief SlowMonster is a monster that slowly walks towards the player and rapidly shoots a lot of projectiles. The aim of slowmonster is purposfully inacurate
 *
 */
class SlowMonster : public Monster {
public:
    SlowMonster(PlayerPS player, float xPos, float yPos);
    SlowMonster(PlayerPS player, sf::Vector2f pos);
    ~SlowMonster();

    virtual std::list<ProjectileUP> Attack();
    virtual bool Move(float dt);

    void initVariables();

private:
};

#endif