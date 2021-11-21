#include "projectile.hpp"

namespace {
const std::string PROJECTILE_SPRITE = "content/sprites/projectile.png";
}

Projectile::Projectile(sf::Vector2i pos, sf::Vector2f size)
    : Entity(PROJECTILE_SPRITE, pos.x, pos.y, size)
{
    initVariables();
}

bool Projectile::move(float dt, float x, float y)
{
    float xMovement = (x / (std::abs(x) + std::abs(y))) * projectileSpeed_;
    float yMovement = (y / (std::abs(x) + std::abs(y))) * projectileSpeed_;
    xMovement *= dt;
    yMovement *= dt;
    xPos_ += xMovement;
    yPos_ += yMovement;
    sprite_.setPosition(xPos_, yPos_);
    return true;
}

void Projectile::Update(float dt)
{
    timeExisted_ += dt;
    float distanceTravelledSquared = (std::abs(startPos_.x - xPos_) + std::abs(startPos_.y - yPos_));
    if (distanceTravelledSquared > distanceLifeSpanSquared_) {
        alive_ = false;
    }
    move(dt, direction_.x, direction_.y);
}

void Projectile::initVariables()
{
    alive_ = true;
    timeExisted_ = 0;
    startPos_ = GetPos();
}

// Getters
int Projectile::GetDamage() { return damage_; }
Projectile::Type Projectile::GetType() { return type_; }
sf::Vector2f Projectile::GetDirection() { return direction_; }
sf::Vector2i Projectile::GetStartPosition() { return startPos_; }
float Projectile::GetTimeExisted() { return timeExisted_; }
float Projectile::GetTimeLifeSpan() { return timeLifeSpan_; }
float Projectile::GetProjectileSpeed() { return projectileSpeed_; }
float Projectile::GetDistanceLifeSpan() { return distanceLifeSpanSquared_; }
bool Projectile::IsAlive() { return alive_; }

// Setters
void Projectile::SetDamage(int damage) { damage_ = damage; }
void Projectile::SetDirection(sf::Vector2f direction) { direction_ = direction; }
void Projectile::SetType(Projectile::Type type) { type_ = type; }
void Projectile::SetTimeLifeSpan(float timeLifeSpan) { timeLifeSpan_ = timeLifeSpan; }
void Projectile::SetDistanceLifeSpan(float distanceLifeSpan) { distanceLifeSpanSquared_ = distanceLifeSpan * distanceLifeSpan; }
void Projectile::SetProjectileSpeed(float projectileSpeed) { projectileSpeed_ = projectileSpeed; }