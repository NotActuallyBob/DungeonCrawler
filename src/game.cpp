#include "game.hpp"
#include "Collision.hpp"

namespace {
const sf::Vector2u VIDEOMODE_DIMS = sf::Vector2u(1280, 768);

}
Game::Game()
{
    room = new RoomInstance(VIDEOMODE_DIMS);
    player_ = new Player();
    SwordWeapon* sword = new SwordWeapon(5, 10, sf::Vector2f(50, 100), 120, "content/sprites/projectiles.png");

    player_->Equip(sword);

    MonsterSpawner spawner = MonsterSpawner(player_, *room, 5);
    auto spawnedMonsters = spawner.SpawnMonsters();
    for (auto m : spawnedMonsters) {
        monsters_.push_back(m);
    }
    gamebar_ = Gamebar(player_);
    initVariables();
    initWindow();
    dtClock.restart(); // to not have giant first dt
}

Game::~Game()
{
    delete window_;
    delete player_;
    for (auto monster : monsters_) {
        delete monster;
    }
}

void Game::UpdateGame()
{
    Events();

    updateDt();
    manageInput();

    // Update projectiles
    for (auto monster : monsters_) {
        // if moved, check collision with walls
        bool monsterMoved = monster->Move(dt);
        if (monsterMoved && collidesWithWall(monster)) {
            monster->RevertMove();
        }

        std::list<Projectile*> projectileListToAdd = monster->Attack();
        addProjectiles(projectileListToAdd);

        monster->Update(dt);
        std::cout << monster->GetPos().x << "," << monster->GetPos().y << std::endl;
    }
    updateProjectiles();
    // checkCollisions(player_, Projectile::Type::EnemyProjectile);
    // handleMonsterProjectileCollisions(monsters_, Projectile::Type::PlayerProjectile);
    checkMonsterCollisions();
    checkPlayerCollisions();
    checkAndHandleProjectileWallCollisions();
    player_->Update(dt);
    gamebar_.Update();
}
// render game frames
void Game::RenderGame()
{
    window_->clear();
    room->Render(window_);
    player_->Render(window_);
    gamebar_.Render(window_);
    for (auto projectile : projectiles_) {
        projectile->Render(window_);
    }
    for (auto monster : monsters_) {
        monster->Render(window_);
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
        default:
            break;
        }
    }
}

void Game::initVariables() { gameEnder_ = false; }
// initalize window
void Game::initWindow()
{
    videomode_ = sf::VideoMode(VIDEOMODE_DIMS.x, VIDEOMODE_DIMS.y);
    window_ = new sf::RenderWindow(videomode_, "Dungeon Crawler");
}

void Game::updateDt() { dt = dtClock.restart().asSeconds(); }

void Game::manageInput()
{
    bool W = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    bool A = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    bool S = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    bool D = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    bool LSHIFT = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
    bool LMOUSE = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    bool twoKeys = ((W || S) && (A || D));
    bool triedMoving = W || A || S || D;

    if (twoKeys) {
        if (A) {
            player_->MoveLeft(dt / sqrt(2));
        } else if (D) {
            player_->MoveRight(dt / sqrt(2));
        }

        if (W) {
            player_->MoveUp(dt / sqrt(2));
        } else if (S) {
            player_->MoveDown(dt / sqrt(2));
        }
    } else {
        if (A) {
            player_->MoveLeft(dt);
        } else if (D) {
            player_->MoveRight(dt);
        }

        if (W) {
            player_->MoveUp(dt);
        } else if (S) {
            player_->MoveDown(dt);
        }
    }

    if (LSHIFT) {
        player_->Dash();
    }

    if (LMOUSE) {
        sf::Vector2f mousePos = window_->mapPixelToCoords(sf::Mouse::getPosition(*window_));
        std::list<Projectile*> projectileListToAdd = player_->Attack(mousePos);
        addProjectiles(projectileListToAdd);
    }
    if (triedMoving) {
        if (collidesWithWall(player_)) {
            player_->RevertMove();
        }
    }
}

void Game::checkCollisions(Character* character, Projectile::Type projectileType)
{
    for (auto projectile : projectiles_) {
        if (projectile->GetType() == projectileType && !projectile->hasHit(character)) {
            if (Collision::PixelPerfectTest(projectile->GetSprite(), character->GetSprite())) {
                projectile->hit(character);
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
    std::vector<Monster*> deadMonsters;
    for (auto monster : monsters_) {
        checkCollisions(monster, Projectile::Type::PlayerProjectile);
        if (!monster->IsAlive()) {
            deadMonsters.push_back(monster);
        }
    }
    for (auto monster : deadMonsters) {
        deleteMonster(monster);
    }
}

void Game::checkPlayerCollisions()
{
    if (projectiles_.empty()) {
        return;
    }
    checkCollisions(player_, Projectile::Type::EnemyProjectile);
}

void Game::checkAndHandleProjectileWallCollisions()
{
    for (auto projectile : projectiles_) {
        if (collidesWithWall(projectile)) {
            projectile->Kill();
        }
    }
}
// redundant atm
void Game::deleteProjectile(Projectile* p)
{
    if (projectiles_.empty())
        return;

    for (auto it = projectiles_.begin(); it != projectiles_.end(); ++it) {
        if (*it == p) {
            projectiles_.erase(it);
            return;
        }
    }
}

void Game::addProjectiles(std::list<Projectile*> projectiles)
{
    if (projectiles.empty()) {
        return;
    }

    for (auto projectile : projectiles) {
        projectiles_.push_back(projectile);
    }
}

void Game::deleteMonster(Character* m)
{
    if (monsters_.empty())
        return;

    for (auto it = monsters_.begin(); it != monsters_.end(); ++it) {
        if (*it == m) {
            monsters_.erase(it);
            return;
        }
    }
}

void Game::updateProjectiles()
{
    if (projectiles_.empty())
        return;

    for (auto it = projectiles_.begin(); it != projectiles_.end(); ++it) {
        auto p = *it;
        if (!p->IsAlive()) {
            it = projectiles_.erase(it);
        } else {
            p->Update(dt);
        }
    }
}
bool Game::collidesWithWall(Character* character)
{
    return !room->positionIsWalkable(character->GetBaseBoxAt(character->GetPos()));
}
bool Game::collidesWithWall(Entity* object)
{
    return !room->positionIsWalkable(object->getSpriteBounds());
}

bool Game::gameLost()
{
    return player_->IsAlive();
}
