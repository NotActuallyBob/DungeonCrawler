#include "Actors/monster.hpp"

Monster::Monster(Player* player, sf::Vector2f pos, const std::string& spriteFile)
    : Character(spriteFile, pos)
    , player_(player)
{
    initVariables();
    sf::RectangleShape healthbar(sf::Vector2f(100, 5));
    healthbar.setFillColor(sf::Color::Green);
    healthbar.setPosition(pos.x + 15, pos.y - 5);
    healthbar_ = healthbar;
}
Monster::Monster(Player* player, float xPos, float yPos, const std::string& spriteFile)
    : Character(spriteFile, sf::Vector2f(xPos, yPos))
    , player_(player)
{
    initVariables();
}
void Monster::initVariables()
{
    movedLastTick = false;
    characterProjectileType = Projectile::Type::EnemyProjectile;
}

float Monster::RandomFloatBetween(float min, float max)
{
    float random = ((float)rand()) / (float)RAND_MAX;
    float diffrence = max - min;
    float r = random * diffrence;
    return min + r;
}

int Monster::RandomIntBetween(int min, int max)
{
    return (rand() % (max - min + 1) + min);
}

Monster::~Monster()
{
}

void Monster::Render(sf::RenderTarget* target)
{
    target->draw(GetSprite());
    target->draw(healthbar_);
}

Player& Monster::GetPlayer() const
{
    return *player_;
}
void Monster::Update(float dt)
{
    if (GetPos() == oldPos_) {
        movedLastTick = false;
    } else {
        movedLastTick = true;
    }
    generalUpdate(dt);

    int width = hitpoints_;
    int newWidth = std::min(100, std::max(0, width));
    healthbar_.setSize(sf::Vector2f(newWidth, 5));
    healthbar_.setPosition(GetPos() + sf::Vector2f(15, -5));

    clampPosToRoom();
}

void Monster::clampPosToRoom()
{
    float max_x = 1130.0f;
    float min_x = 70.0f;
    if (pos_.x > max_x) {
        SetPos(sf::Vector2f(max_x, pos_.y));
    } else if (pos_.x < min_x) {
        SetPos(sf::Vector2f(min_x, pos_.y));
    }

    float max_y = 620.0f;
    float min_y = 100.0f;
    if (pos_.y > max_y) {
        SetPos(sf::Vector2f(pos_.x, max_y));
    } else if (pos_.y < min_y) {
        SetPos(sf::Vector2f(pos_.x, min_y));
    }
}

float Monster::getDistanceToPlayer()
{
    sf::Vector2f playerpos = GetPlayer().GetSpriteCenter();
    sf::Vector2f distanceVec = playerpos - GetSpriteCenter();
    float distance = std::sqrt(distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y);
    return distance;
}