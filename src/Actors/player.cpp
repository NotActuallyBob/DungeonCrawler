#include "Actors/player.hpp"

namespace {
const std::string PLAYER_SPRITE = "content/sprites/characters/SpriteSheet.png";
}

Player::Player()
    : Character(PLAYER_SPRITE, sf::Vector2f(200, 200), true)
{
    initVariables();
}

int Player::GetHitPoints() const { return hitpoints_; }

void Player::Update(float dt)
{
    if (hasAnimation_) {
        if (oldPos_.x == pos_.x && oldPos_.y == pos_.y) {
            Idle();
        }
        Animations[int(currentAnimation)]->Update(dt);
        Animations[int(currentAnimation)]->AnimationToSprite(sprite_);
    }

    generalUpdate(dt);

    updateDashCooldown(dt);

    if (IsDashing) {
        currentSpeed_ = dashSpeed;
    } else {
        currentSpeed_ = normalSpeed_;
    }
}

void Player::Dash()
{
    if (CanDash) {
        IsDashing = true;
        dashDurationLeft = dashDurationLength;
        ResetDashCooldown();
    }
}

void Player::initVariables()
{
    dashSpeed = 400.0f;
    dashCooldownLength = 1.0f;
    dashCooldownLeft = 0.0f;
    CanDash = true;
    IsDashing = false;
    dashDurationLength = 1.0f;
    dashDurationLeft = 0.0f;

    characterProjectileType = Projectile::Type::PlayerProjectile;
}

void Player::ResetDashCooldown()
{
    dashCooldownLeft = dashCooldownLength;
    CanDash = false;
}

void Player::updateDashCooldown(float dt)
{
    dashCooldownLeft = std::max(0.0f, dashCooldownLeft - dt);
    if (dashCooldownLeft <= 0.0f) {
        CanDash = true;
    }

    if (IsDashing) {
        dashDurationLeft -= dt;
    }
    if (dashDurationLeft <= 0) {
        IsDashing = false;
    }
}

std::list<Projectile*> Player::Attack(sf::Vector2f aimPos)
{
    if (!CanAttack) {
        return emptyList();
    }

    if (weapon_ == nullptr) {
        return emptyList();
    }

    ResetAttackCooldown();

    return shotProjectileList(aimPos);
}