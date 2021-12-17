#include "game.hpp"
#include "Utility/Collision.hpp"
#define C_PIXELS 64

namespace {
const sf::Vector2u VIDEOMODE_DIMS = sf::Vector2u(1280, 768);
const std::string DEATHTEXT = "content/sprites/DEATHTEXT.png";
const std::string VICTORY = "content/endscreen.png";
}

Game::Game()
    : player_(PlayerPS(new Player()))
    , dungeonMap_(Map(VIDEOMODE_DIMS, 10, player_))
    , gamebar_(Gamebar(player_))
    , deathtext_(ScreenText(DEATHTEXT, { 0, 0 }, { 3, 3 }))
    , victoryScreen_(ScreenText(VICTORY, { 0, 0 }, { 7, 6 }))
    , gameEnder_(false)
{
    SwordWeapon* playerSword = new SwordWeapon(20, 100, 120, 1000, sf::Vector2f(200, 200), "content/sprites/Weapons/swordtoobig.png");

    player_->Equip(playerSword);
    LevelUpSystem::AddCharacter(player_.get());
    initWindow();
    dtClock_.restart(); // to not have giant first dt
}

Game::~Game()
{
    delete window_;
    delete playerHitSound;
    delete monsterHitSound;
}

void Game::UpdateGame()
{
    Events();
    managePauseInput();

    if (!paused) {
        updateDt();
        manageInput();
        updateMonsters();
        updatePotions();
        updateProjectiles();
        checkMonsterCollisions();
        checkPlayerCollisions();
        checkAndHandleProjectileWallCollisions();

        player_->Update(dt_);
        gamebar_.Update();
    }
}
// render game frames
void Game::RenderGame()
{
    window_->clear();

    if (gameWon()) {
        victoryScreen_.Render(window_);
    } else {
        dungeonMap_.RenderCurrentRoom(window_);
        player_->Render(window_);
        gamebar_.Render(window_);
        for (auto& projectile : projectiles_) {
            projectile->Render(window_);
        }
        for (auto& monster : dungeonMap_.GetCurrentRoom()->GetMonsters()) {
            monster->Render(window_);
        }
        if (gameLost() == true) {
            deathtext_.Render(window_);
        }
        for (auto potion : dungeonMap_.GetCurrentRoom()->GetPotions()) {
            potion->Render(window_);
        }
    }
    window_->display();
}
bool Game::Running() const { return window_->isOpen(); }

void Game::Events()
{
    while (window_->pollEvent(event_)) {
        switch (event_.type) {
        case sf::Event::Closed:
            window_->close();
            break;
        case sf::Event::LostFocus:
            paused = true;
            break;
        case sf::Event::GainedFocus:
            paused = false;
            break;
        case sf::Event::KeyPressed:
            if (event_.key.code == sf::Keyboard::Num1)
                player_->UsePotion("red");
            else if (event_.key.code == sf::Keyboard::Num2)
                player_->UsePotion("green");
            else if (event_.key.code == sf::Keyboard::Num3)
                player_->UsePotion("yellow");
            else if (event_.key.code == sf::Keyboard::Num4)
                player_->UsePotion("violet");
            break;
        default:
            break;
        }
    }
}
// initalize window
void Game::initWindow()
{
    // gets resolution and etc.
    auto desktopMode = sf::VideoMode::getDesktopMode();
    videomode_ = sf::VideoMode(std::min(VIDEOMODE_DIMS.x, desktopMode.width), std::min(VIDEOMODE_DIMS.y, desktopMode.height));
    window_ = new sf::RenderWindow(videomode_, "Dungeon Crawler");
}

void Game::updateDt() { dt_ = dtClock_.restart().asSeconds(); }

void Game::manageInput()
{
    bool W = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    bool A = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    bool S = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    bool D = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    bool LSHIFT = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
    bool LMOUSE = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    bool ENTER = sf::Keyboard::isKeyPressed(sf::Keyboard::Return);
    bool twoKeys = ((W || S) && (A || D));
    bool triedMoving = W || A || S || D;

    if (twoKeys) {
        if (A) {
            player_->MoveLeft(dt_ / sqrt(2));
        } else if (D) {
            player_->MoveRight(dt_ / sqrt(2));
        }

        if (W) {
            player_->MoveUp(dt_ / sqrt(2));
        } else if (S) {
            player_->MoveDown(dt_ / sqrt(2));
        }
    } else {
        if (A) {
            player_->MoveLeft(dt_);
        } else if (D) {
            player_->MoveRight(dt_);
        }

        if (W) {
            player_->MoveUp(dt_);
        } else if (S) {
            player_->MoveDown(dt_);
        }
    }

    if (LSHIFT) {
        player_->Dash();
    }

    if (LMOUSE) {
        sf::Vector2f mousePos = window_->mapPixelToCoords(sf::Mouse::getPosition(*window_));
        addProjectiles(player_->Attack(mousePos));
    }
    if (ENTER) {
        if (!gameLost()) {
        } else {
            restartGame();
        }
    }

    if (triedMoving) {
        // std::cout << player_->GetPos().x << " " << player_->GetPos().y << std::endl;
        if (collidesWithWall(player_.get())) {
            player_->RevertMove();
        }
        if (ShouldChangeRoom()) {
            projectiles_.clear();
            dtClock_.restart(); // generating monsters makes dt quite big
        }
    }
}

void Game::managePauseInput()
{
    bool ESCAPE = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);

    if (ESCAPE) {
        if (escapePressedLastTick != ESCAPE) {
            paused = !paused;
            if (!paused) {
                dtClock_.restart();
            }
        }
    }
    escapePressedLastTick = ESCAPE;
}

void Game::checkCollisions(Character* character, Projectile::Type projectileType)
{
    for (auto& projectile : projectiles_) {
        if (projectile->GetType() == projectileType && !projectile->HasHit(character)) {
            if (Collision::PixelPerfectTest(projectile->GetSprite(), character->GetSprite())) {
                if (projectileType == Projectile::Type::PlayerProjectile) {
                    monsterHitSound->PlaySound();
                } else if (projectileType == Projectile::Type::EnemyProjectile && player_->IsAlive()) {
                    playerHitSound->PlaySound();
                }
                projectile->Hit(character);
                character->TakeDamage(projectile->GetDamage());
                if (!projectile->Penetrates()) {
                    projectile->Kill();
                }
            }
        }
    }
}

void Game::checkMonsterCollisions()
{
    if (projectiles_.empty()) {
        return;
    }
    auto currentRoom = dungeonMap_.GetCurrentRoom();
    std::vector<MonsterSP> deadMonsters;
    for (auto& monster : currentRoom->GetMonsters()) {
        checkCollisions(monster.get(), Projectile::Type::PlayerProjectile);
        if (!monster->IsAlive()) {
            deadMonsters.push_back(monster);
            Potion* potion = monster->ReturnPotion();
            if (potion != nullptr) {
                dungeonMap_.GetCurrentRoom()->AddPotion(potion);
            }
        }
    }
    for (auto monster : deadMonsters) {
        LevelUpSystem::GainXP(player_.get(), 5.0f);
        currentRoom->deleteMonster(monster);
    }
}

void Game::checkPlayerCollisions()
{
    if (projectiles_.empty()) {
        return;
    }
    checkCollisions(player_.get(), Projectile::Type::EnemyProjectile);
}

void Game::checkAndHandleProjectileWallCollisions()
{
    for (auto& projectile : projectiles_) {
        if (collidesWithWall(projectile.get())) {
            projectile->Kill();
        }
    }
}

void Game::addProjectiles(std::list<ProjectileUP> projectiles)
{
    if (projectiles.empty()) {
        return;
    }

    for (auto& projectile : projectiles) {
        projectiles_.push_back(std::move(projectile));
    }
}

void Game::deletePotion(Potion* p)
{
    auto& potions = dungeonMap_.GetCurrentRoom()->GetPotions();
    if (potions.empty())
        return;

    for (auto it = potions.begin(); it != potions.end(); ++it) {
        if (*it == p) {
            it = potions.erase(it);
            return;
        }
    }
}

void Game::updateProjectiles()
{
    if (projectiles_.empty())
        return;

    for (auto it = projectiles_.begin(); it != projectiles_.end(); ++it) {
        auto& p = *it;
        if (!p->IsAlive()) {
            it = projectiles_.erase(it);
        } else {
            p->Update(dt_);
        }
    }
}

void Game::updateMonsters()
{
    for (auto monster : dungeonMap_.GetCurrentRoom()->GetMonsters()) {
        // if moved, check collision with walls
        bool monsterMoved = monster->Move(dt_);
        if (monsterMoved && collidesWithWall(monster.get())) {
            monster->RevertMove();
        }
        addProjectiles(monster->Attack());
        monster->Update(dt_);
    }
}

void Game::updatePotions()
{
    for (auto potion : dungeonMap_.GetCurrentRoom()->GetPotions()) {
        sf::Vector2f poPos = potion->GetSpriteCenter();
        sf::Vector2f plPos = player_->GetSpriteCenter();
        sf::Vector2f difference = poPos - plPos;
        float distance = std::sqrt(difference.x * difference.x + difference.y * difference.y);
        if (distance < 50) {
            player_->AddPotion(potion);
            deletePotion(potion);
        }
    }
}

bool Game::collidesWithWall(Character* character)
{
    return !dungeonMap_.GetCurrentRoom()->positionIsWalkable(character->GetBaseBoxAt(character->GetPos()));
}
bool Game::collidesWithWall(Projectile* object)
{
    return !dungeonMap_.GetCurrentRoom()->positionIsPenetratable(object->GetSpriteBounds());
}

bool Game::ShouldChangeRoom()
{
    if (videomode_.width < player_->GetPos().x) {
        dungeonMap_.MovePlayer(Direction::Right);
        return true;
    } else if (player_->GetPos().x + player_->GetSpriteBounds().width < 0) {
        dungeonMap_.MovePlayer(Direction::Left);
        return true;
    } else if (player_->GetPos().y + player_->GetSpriteBounds().height < 0) {
        dungeonMap_.MovePlayer(Direction::Up);
        return true;
    } else if (player_->GetPos().y > videomode_.height) {
        dungeonMap_.MovePlayer(Direction::Down);
        return true;
    }
    return false;
}

bool Game::gameLost()
{
    if (!player_->IsAlive()) {
        gameEnder_ = true;
    }
    return gameEnder_;
}

void Game::restartGame()
{
    dungeonMap_.ResetMap();
    gameEnder_ = false;
    projectiles_.clear();
    player_->ClearInventory();
    player_->ResetCharacterToBeAlive();
}
bool Game::gameWon()
{
    return dungeonMap_.IsBossRoomCleared();
}