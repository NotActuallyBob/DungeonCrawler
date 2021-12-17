#pragma once

#ifndef _CHARACTER_CLASS_
#define _CHARACTER_CLASS_

#include "Combat/Projectile.hpp"
//#include "Interfaces/ICollidable.hpp"
#include "Animation/Animationhandler.hpp"
#include "Combat/Weapons/Weapon.hpp"
#include "Utility/LevelUpSystem.hpp"
#include "Utility/RandomHelper.hpp"
#include "entity.hpp"
/**
 * @brief Character class that our player and monster inherits.
 *
 */
class Character : public Entity /*, public ICollidable*/ {
public:
    Character(const std::string& filename, sf::Vector2f pos, bool animated = false);

    virtual ~Character();

    virtual void Update(float dt) = 0;
    /**
     * @brief Equips weapon to character
     *
     * @param    weapon               weapon to be equipped
     */
    void Equip(Weapon* weapon);

    void initVariables();
    /**
     * @brief removes hitpoints corresponding to the damage taken value
     *
     * @param    value                damage taken
     */
    void TakeDamage(int value);
    /**
     * @brief Heal character
     *
     * @param    value                value healed
     */
    void Heal(int value);
    int GetHitPoints() const;
    /**
     * @brief Check if player is alive
     *
     * @return true if alive
     * @return false if not
     */
    bool IsAlive();
    /**
     * @brief Check if player has weapon equipped
     *
     * @return true if has
     * @return false if not
     */
    bool HasWeapon();
    /**
     * @brief Set animation to IDLE
     *
     * @return true when set
     *
     */
    bool Idle();
    /**
     * @brief Set animation to DEAD
     *
     * @return true when set
     *
     */
    bool Dead();
    /**
     * @brief Sets the animation to right direction (LEFT) and moves the character.
     *
     * @param    dt                   deltatime
     * @return true when set
     *
     */
    bool MoveLeft(float dt);
     /**
     * @brief Sets the animation to right direction (RIGHT) and moves the character.
     *
     * @param    dt                   deltatime
     * @return true when set
     *
     */
    bool MoveRight(float dt);
     /**
     * @brief Sets the animation to right direction (DOWN) and moves the character.
     *
     * @param    dt                   deltatime
     * @return true when set
     *
     */
    bool MoveDown(float dt);
     /**
     * @brief Sets the animation to right direction (UP) and moves the character.
     *
     * @param    dt                   deltatime
     * @return true when set
     *
     */
    bool MoveUp(float dt);
    // For subclasses, should be = 0
    virtual bool Move(float)
    {
        return false;
    }
    /**
     * @brief revert movement to last position, used when hit wall
     *
     */
    void RevertMove();

    void ResetAttackCooldown();
    float GetAttackCooldownLeft() const { return attackCooldownLeft; };
    float GetAttackCooldownLength() const { return attackCooldownLength_; };
    bool CanAttack;
    int GetMaxHP();

    void SetNormalSpeed(float value);
    void ResetCharacterToBeAlive();
    /*
    // for ICollidable
    virtual sf::FloatRect GetBoundingBox() { return sprite_.getGlobalBounds(); }
    virtual void ProcessCollision(ICollidable* object);
    virtual ICollidable::EntityType GetEntityType();
    */

protected:
    /*void GetHitBy(Projectile& projectile);*/

    Weapon* weapon_;
    Projectile::Type characterProjectileType_;

    sf::Vector2f startPos;

    int hitpoints_;
    int currentMaxHitpoints_;
    int defaultMaxHitpoints_;

    bool hasAnimation_;
    AnimationHandler animationHandler_;
    float currentSpeed_;
    float defaultSpeed_;
    bool left_or_right_ = true;
    void generalUpdate(float dt);
    bool invincibility_frame_ = false;
    float attackCooldownLength_;
    float attackCooldownLeft;
    void updateAttackCooldown(float dt);
    std::list<ProjectileUP> emptyList();
    std::list<ProjectileUP> shotProjectileList(sf::Vector2f aimPos);
};
#endif
