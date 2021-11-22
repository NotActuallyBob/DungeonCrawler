#pragma once
#ifndef _WEAPON_CLASS_
#define _WEAPON_CLASS_

#include "../PowerUps/PowerUp.hpp"
#include "../Projectile.hpp"
#include "SpriteHelper.hpp"

using namespace std;
using namespace sf;
class Weapon {
protected:
    int damage_;
    int range_;
    Vector2f projectileSize_;
    bool penetrates_ = false;
    const int maxPowerUps = 3;
    int getPowerUpCount();
    Sprite sprite_;
    vector<PowerUp*> powerUps_;

public:
    Weapon(int damage, int range, Vector2f projectileSize, const std::string& spriteLocation);
    virtual ~Weapon() {};
    virtual list<Projectile*> Use(Vector2f dir, Vector2f origin) = 0;
    // virtual void Animate() = 0;
    void AddPowerUp(PowerUp* up);
};
#endif