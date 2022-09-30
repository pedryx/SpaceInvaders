#include "Entity.hpp"

#include <stdlib.h>

unsigned int Entity::lastId_ = 0;

#include <iostream>

void Entity::update(const sf::Time& elapsed)
{
    // update positon
    sf::Vector2f pos = sprite_->getPos();
    pos.x += speed_.x * elapsed.asMilliseconds();
    pos.y += speed_.y * elapsed.asMilliseconds();
    sprite_->setPos(pos);
}

void Entity::render(const sf::Time& elapsed, sf::RenderWindow& window)
{
    sprite_->render(window);
}

Rectf Entity::getRigid() const
{
    Rectf rect;
    rect.left = sprite_->getPos().x - sprite_->getSize().x / 2 - speed_.x;
    rect.top = sprite_->getPos().y - sprite_->getSize().y / 2 - speed_.y;
    rect.width = sprite_->getSize().x + abs(speed_.x);
    rect.height = sprite_->getSize().y + abs(speed_.y);

    return rect;
}
