#include "RandomMonster.hpp"
namespace {
const std::string monsterSpriteFileLocation = "content/sprites/monster1.png";
}

RandomMonster::RandomMonster(Player& player, sf::Vector2f pos)
    : Monster(player, pos, monsterSpriteFileLocation)
{
}
RandomMonster::RandomMonster(Player& player, float xPos, float yPos)
    : Monster(player, sf::Vector2f(xPos, yPos), monsterSpriteFileLocation)
{
    initVariables();
}

RandomMonster::~RandomMonster() { }

bool RandomMonster::Move(float dt)
{
    elapsedTurnTime_ += dt;
    if (elapsedTurnTime_ > durationUntilTurn_ || !movedLastTick_) {
        elapsedTurnTime_ = 0.0f;
        currentDir_ = RandomIntBetween(1, 4);
        durationUntilTurn_ = RandomFloatBetween(0.2f, 1.0f);
    }
    if (currentDir_ == 1) {
        MoveDown(dt);
    } else if (currentDir_ == 2) {
        MoveLeft(dt);
    } else if (currentDir_ == 3) {
        MoveUp(dt);
    } else {
        MoveRight(dt);
    }
    return true;
}

std::list<Projectile*> RandomMonster::Attack()
{
    if (!CanAttack || !HasWeapon() || !inRangeOfPlayer()) {
        return emptyList();
    }

    ResetAttackCooldown();

    return shotProjectileList(player_.GetSpriteCenter());
}

void RandomMonster::initVariables()
{
    SetNormalSpeed(200.0f);
}