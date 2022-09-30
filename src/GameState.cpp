#include "GameState.hpp"
#include "Entity.hpp"
#include "Entities.hpp"

void GameState::update(const sf::Time& elapsed)
{
    // update entities
    for (auto&& entity : entities_)
        entity.second->update(elapsed);

    // remove netities
    for (auto&& id : toRemove_)
    {
        // ui entitiy
        auto it = uiEntities_.find(id);
        if (it != uiEntities_.end())
        {
            uiEntities_[id]->onDead();
            uiEntities_.erase(id);
            continue;
        }

        // life bar
        LBarPtr lifeBar = entities_[id]->getLifeBar();
        if (lifeBar == nullptr)
        {
            entities_[id]->onDead();
            entities_.erase(id);
        }
        else
        {
            lifeBar->decreseCount();
            if (lifeBar->getCount() == 0)
            {
                entities_[id]->onDead();
                entities_[id]->setLifeBar(nullptr);
                entities_.erase(id);
                entities_.erase(lifeBar->getId());
            }
            else
                entities_[id]->respawn();
        }
    }
    // add entities
    for (auto&& entity : toAdd_)
    {
        if (static_cast<bool>(entity->getType() & EntityType::UI))
            uiEntities_.insert(std::make_pair(entity->getId(), entity));
        else
            entities_.insert(std::make_pair(entity->getId(), entity));
    }

    toRemove_.clear();
    toAdd_.clear();
}

void GameState::render(const sf::Time& elapsed, sf::RenderWindow& window)
{
    for (auto&& entity : entities_)
        entity.second->render(elapsed, window);

    for (auto&& entity : uiEntities_)
        entity.second->render(elapsed, window);
}

void GameState::addEntity(EntityPtr entity) 
{ 
    toAdd_.push_back(entity);
}

void GameState::removeEntity(unsigned int index)
{
    toRemove_.push_back(index);
}

std::vector<unsigned int> GameState::collision(unsigned int id, EntityType target) const
{
    std::vector<unsigned int> collided;
    Rectf rect = entities_.find(id)->second->getRigid();

    for (auto&& entity : entities_)
    {
        if (entity.first == id)
            continue;

        if (!static_cast<bool>(entity.second->getType() & target))
            continue;

        if (rect.intersects(entity.second->getRigid()))
            collided.push_back(entity.first);
    }

    return collided;
}

std::shared_ptr<Entity> GameState::getEntity(unsigned int id)
{
    auto it = uiEntities_.find(id);
    if (it != uiEntities_.end())
        return uiEntities_[id];
    else
        return entities_[id];
}