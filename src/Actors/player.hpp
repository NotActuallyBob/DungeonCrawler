#pragma once

#ifndef _PLAYER_CLASS_
#define _PLAYER_CLASS_

#include "Actors/character.hpp"
#include "Combat/Health/Potion.hpp"
/**
 * @brief Player class, our controlled character
 *
 */
class Player : public Character {
public:
    Player();
    ~Player() {};

    virtual void Update(float dt);
    /**
     * @brief Handles the dashing functions
     *
     */
    void Dash();

    /**
     * @brief handles the attacking of player character
     *
     * @param    aimPos               position of where mouse is aiming
     * @return std::list<ProjectileUP>
     */
    std::list<ProjectileUP> Attack(sf::Vector2f aimPos);

    void initVariables();

    void ResetDashCooldown();

    float GetDashCooldownLeft() const { return dashCooldownLeft_; };
    float GetDashCooldownLength() const { return dashCooldownLength_; };

    bool CanDash;
    bool IsDashing;
    /**
     * @brief Adds a potion to be used later
     *
     * @param    potion               potion to be added to inventory
     */
    void AddPotion(Potion* potion);
    /**
     * @brief Get the colour of the potion player is going to use and runs the funcions to use it
     *
     * @param    colour               colour of potion
     */
    void UsePotion(const std::string& colour);
    /**
     * @brief gets inventory of
     *
     * @return std::vector<Potion*>
     */
    std::vector<Potion*> GetInventory() const;
    void ClearInventory();

private:
    int attacksBoosted_;
    int dashesBoosted_;

    float dashSpeed_;
    float dashLengthBoostModifier = 2.0f;
    float dashDefaultDurationLength_;
    float dashCurrentDurationLength_;
    float dashDurationLeft_;
    bool deadAnimationPlayed = false;
    float dt_time = 0;
    float dashCooldownLength_;
    float dashCooldownLeft_;
    void updateDashCooldown(float dt);

    std::vector<Potion*> inventory_;
};
typedef std::shared_ptr<Player> PlayerPS;

#endif