#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>
#include <vector>
#include <type_traits>

/**
 * Represent a type of entity.
 */
enum class EntityType : unsigned int
{
    None = 0x0,
    Player = 0x1,
    Structure = 0x2,
    Enemy = 0x4,
    UI = 0x8,
};

inline EntityType operator | (EntityType left, EntityType right)
{
    return static_cast<EntityType>(
        static_cast<std::underlying_type<EntityType>::type>(left) |
        static_cast<std::underlying_type<EntityType>::type>(right)
    );
}

inline EntityType operator & (EntityType left, EntityType right)
{
    return static_cast<EntityType>(
        static_cast<std::underlying_type<EntityType>::type>(left) &
        static_cast<std::underlying_type<EntityType>::type>(right)
    );
}

class Entity;
class Game;
class ScoreBar;

/**
 * Represent a game state.
 */
class GameState
{
public:
    virtual ~GameState() {}

    /**
     * Initialize game state.
     * 
     */
    virtual void init() = 0;
    /**
     * Update game state's logic.
     * 
     * \param elapsed Ellased time from last update.
     */
    virtual void update(const sf::Time& elapsed);
    /**
     * Render game state.
     * 
     * \param elapsed Ellapsed time form last update.
     * \param window Window used for rendering.
     */
    virtual void render(const sf::Time& elapsed, sf::RenderWindow& window);
    /**
     * Add new entity to a game state.
     * 
     * \param entity Entitiy to add.
     */
    void addEntity(std::shared_ptr<Entity> entity);
    /**
     * Remove entity from a game state.
     * 
     * \param index Index of entitiy to remove.
     */
    void removeEntity(unsigned int index);
    
    /**
     * Check for collision between entitiy of speicifc id and all entities of specific type..
     * 
     * \param id Id of entitiy to check.
     * \param target Type of all entitis to check.
     * \return Vector which contains id of all collided entities (not a checked entitity).
     */
    std::vector<unsigned int> collision(unsigned int id, EntityType target) const;
    Game* getGame() { return game_; }
    std::shared_ptr<ScoreBar> getScoreBar() { return scoreBar_; }
    std::shared_ptr<Entity> getEntity(unsigned int id);
protected:
    GameState(Game* game) : game_(game) {}
    std::shared_ptr<ScoreBar> scoreBar_;
private:
    std::map<unsigned int, std::shared_ptr<Entity>> entities_;
    std::map<unsigned int, std::shared_ptr<Entity>> uiEntities_;
    /**
     * Contains id of all entities which will be removed on the end of iteration.
     */
    std::vector<unsigned int> toRemove_;
    /**
     * Contains all entities which will be added next iteration.
     */
    std::vector<std::shared_ptr<Entity>> toAdd_;
    Game* game_;
};

using GameStatePtr = std::shared_ptr<GameState>;

#endif // !GAMESTATE_HPP