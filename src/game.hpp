#pragma once

#ifndef _GAME_CLASS_
#define _GAME_CLASS_

#include "Actors/Monsters/BossMonster.hpp"
#include "Actors/player.hpp"
#include "Combat/Health/Potion.hpp"
#include "Combat/Projectile.hpp"
#include "Combat/Weapons/BowWeapon.hpp"
#include "Combat/Weapons/SwordWeapon.hpp"
#include "Dungeon/map.hpp"
#include "Utility/LevelUpSystem.hpp"
#include "Utility/ScreenText.hpp"
#include "Utility/Sounds/SoundEffects.hpp"
#include "gamebar.hpp"

class Game {
public:
    Game();
    ~Game();
    /**
     * @brief Updates the game instance/variables in the game loop
     *
     */
    void UpdateGame();
    /**
     * @brief Renders the updated game variables in the game loop
     *
     */
    void RenderGame();
    /**
     * @brief Checks that the game is running aka window is open
     *
     * @return true/false based if the window is open
     *
     */
    bool Running() const;
    /**
     * @brief pulls events such as if the window is closed
     *
     */
    void Events();

private:
    sf::VideoMode videomode_;
    sf::RenderWindow* window_;
    sf::Event event_;
    sf::Clock dtClock_;
    PlayerPS player_;
    Map dungeonMap_;
    Gamebar gamebar_;
    ScreenText deathtext_;
    ScreenText victoryScreen_;

    SoundEffect* playerHitSound = new SoundEffect("content/sounds/playerHit.wav");
    SoundEffect* monsterHitSound = new SoundEffect("content/sounds/monsterHit.wav");

    float dt_;
    bool paused = false;
    bool escapePressedLastTick = paused;
    bool gameEnder_;

    std::list<ProjectileUP> projectiles_;

    /**
     * @brief initializes game window
     *
     */
    void initWindow();
    /**
     * @brief updates the Deltatime (dt_) variable to compensate for diffrent frame rates on diffrent computers
     *
     */
    void updateDt();
    /**
     * @brief manages inputs, like keypresses (except for esc for pause) and player movement and runs the relevant functions
     *
     */
    void manageInput();
    /**
     * @brief manages input for the Escape key for pausing the game
     *
     */
    void managePauseInput();
    /**
     * @brief Checks if projectile collides with a character, if hit runs the function for damaging the character
     *
     * @param    character            character that is hit
     * @param    projectileType       the shoot projectile
     */
    void checkCollisions(Character* character, Projectile::Type projectileType);
    /**
     * @brief checks if projectiles hit monsters, removes the monster if they die
     *
     */
    void checkMonsterCollisions();
    /**
     * @brief checks if projectiles hit player
     *
     */
    void checkPlayerCollisions();
    /**
     * @brief removes projectile if it hits wall
     *
     */
    void checkAndHandleProjectileWallCollisions();
    /**
     * @brief removes projectile from projectile list
     *
     * @param    p                    projectile to be deleted
     */
    void deleteProjectile(ProjectileUP p);
    /**
     * @brief adds projectile to projectile list
     *
     * @param    listToAdd            the projectiles to be added
     */
    void addProjectiles(std::list<ProjectileUP> listToAdd);
    /**
     * @brief deletes monster from room if dead
     *
     * @param    m                    monster that is deleted if it's dead
     */
    void deleteMonster(Character* m);
    void deletePotion(Potion* p);
    /**
     * @brief handles the projectiles life span
     *
     */
    void updateProjectiles();
    /**
     * @brief Updates all monsters in the game
     *
     */
    void updateMonsters();
    /**
     * @brief check if character collides with wall
     *
     * @param    character            character that is checked
     * @return true if collides
     * @return false if it does not collide
     */
    void updatePotions();
    bool collidesWithWall(Character* character);

    /**
     * @brief check if projectile collides with wall
     *
     * @param    object               projectile that is checked
     * @return true true if collides
     * @return false false if it does not collide
     */
    bool collidesWithWall(Projectile* object);
    /**
     * @brief checks if player is out of room and gets the direction to handle which room should be loaded next for the player
     *
     * @return true if should change room
     * @return false if not
     */
    bool ShouldChangeRoom();
    /**
     * @brief check if player is dead, changes gameEnder_ to true if player is dead
     *
     * @return true if game has ended
     * @return false if still going
     */
    bool gameLost();
    /**
     * @brief reset game after death, keeps the same player, same map
     *
     */
    void restartGame();

    /**
     * @brief Checks if game is won i.e. if the maps's bossroom has been cleared.
     *
     * @return true if room is cleared
     * @return false not yet cleared
     */
    bool gameWon();
};

#endif
