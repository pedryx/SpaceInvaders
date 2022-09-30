#include "Entities.hpp"
#include "Game.hpp"
#include "GameStates/LevelState.hpp"

#include <cstdlib>
#include <windows.h>

#define WIN32_LEAN_AND_MEAN

void Enemy::update(const sf::Time& elapsed)
{
    sf::Vector2f pos = getSprite()->getPos();

    // Determine if enemy should fire a projectile
    if (((float)rand() / RAND_MAX) <= fireChance_)
    {
        // Create a projectile entitiy and fire it.
        float speed = .6f;
        IRenderPtr sprite = std::make_shared<RectSprite>(
            pos, sf::Vector2f(3.f, 5.f), sf::Color::Green
        );
        EntityType target = EntityType::Player | EntityType::Structure;
        getGameState()->addEntity(std::make_shared<Projectile>(
            getGameState(), sprite, speed, target
        ));
    }

    // movement
    if (move_)
    {
        if (down_)
        {
            pos.y += abs(distance_);
            down_ = false;
        }
        else
        {
            pos.x += distance_;
        }

        move_ = false;
        needed_ -= timeAcc_;
        fireChance_ += fireAcc_;
        sprite_->NextFrame();
    }
    else
    {
        total_ += elapsed.asMilliseconds();
        if (total_ >= needed_)
        {
            move_ = true;
            total_ -= needed_;
            steps_++;
            if (steps_ > maxSteps_)
            {
                steps_ = 0;
                distance_ *= -1;
                down_ = true;
            }
        }
    }

    getSprite()->setPos(pos);
}

void Enemy::onDead()
{
    auto levelState = dynamic_cast<LevelState*>(getGameState());

    levelState->clusterSize_--;
    // check for game over
    if (levelState->clusterSize_ == 0)
    {
        MessageBoxA(
            NULL,
            "Game over!\nYou win.",
            "GAME OVER",
            MB_OK
        );
        levelState->getGame()->exit();
    }
}

void Projectile::update(const sf::Time& elapsed)
{
    Entity::update(elapsed);

    // check for borders
    sf::Vector2f pos = getSprite()->getPos();
    if (pos.y <= 0.f || pos.y >= 800.f)
        getGameState()->removeEntity(getId());

    // check for collisions
    std::vector<unsigned int> collided = getGameState()->collision(getId(), target_);
    if (collided.size() != 0)
    {
        std::vector<unsigned int>::iterator it;
        if (speed_.y < 0)
            it = std::max_element(collided.begin(), collided.end());
        else
            it = std::min_element(collided.begin(), collided.end());

        int score = getGameState()->getEntity(*it)->getScore();
        if (score != 0)
            getGameState()->getScoreBar()->addScore(score);

        getGameState()->removeEntity(*it);
        getGameState()->removeEntity(getId());
    }
}

void Player::update(const sf::Time& elapsed)
{
    Entity::update(elapsed);

    // control handling
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        speed_.x = -.4f;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        speed_.x = .4f;
    else
        speed_.x = 0.f;

    // check for borders
    sf::Vector2f pos = getSprite()->getPos();
    if (pos.x > 800.f)
        pos.x = 800.f;
    else if (pos.x < 0.f)
        pos.x = 0.f;
    getSprite()->setPos(pos);

    // projectile firing
    if (charge_)
        total_ += elapsed.asMilliseconds();
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        float speed = -.6f;
        EntityType target = EntityType::Structure | EntityType::Enemy;
        IRenderPtr sprite = std::make_shared<RectSprite>(
            pos, sf::Vector2f(3.f, 5.f), sf::Color::Green
        );
        getGameState()->addEntity(std::make_shared<Projectile>(
            getGameState(), sprite, speed, target
        ));
        charge_ = true;
    }

    if (total_ >= needed_)
    {
        total_ -= needed_;
        charge_ = false;
    }

    getSprite()->setPos(pos);
}

void Player::respawn()
{
    sf::Vector2f pos = getSprite()->getPos();
    pos.x = 400.f;
    getSprite()->setPos(pos);
}

void Player::onDead()
{
    MessageBoxA(
        NULL,
        "Game over!\nYou lose.",
        "GAME OVER",
        MB_OK
    );
    getGameState()->getGame()->exit();
}

void LifeBar::render(const sf::Time& elapsed, sf::RenderWindow& window)
{
    IRenderPtr sprite = getSprite();
    sf::Vector2f pos = sprite->getPos();

    // render each life
    for (int i = 0; i < count_; ++i)
    {
        sf::Vector2f newPos = pos;
        newPos.x = pos.x + sprite->getSize().x * 1.2f * i;
        sprite->setPos(newPos);

        sprite->render(window);
    }

    sprite->setPos(pos);
}

bool Ufo::spawned = false;

void Ufo::update(const sf::Time& elapsed)
{
    Entity::update(elapsed);

    // check for border
    if (getSprite()->getPos().x < 0)
    {
        getGameState()->removeEntity(getId());
        spawned = false;
    }
}