#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <memory>

#include "GameState.hpp"
#include "IRenderable.hpp"

using Rectf = sf::Rect<float>;
class LifeBar;

/**
 * Represent a base class for entites.
 */
class Entity
{
public:
    /**
     * Create new entitiy.
     * 
     * \param state Entity's owner.
     * \param sprite Entity's sprite.
     * \param type Entitiy's type.
     */
    Entity(GameState* state, IRenderPtr sprite, const EntityType type)
        : state_(state), sprite_(sprite), type_(type) {}
    virtual ~Entity() {}

    /**
     * Update entity's state.
     * 
     * \param elapsed Ellapsed time from last update.
     */
    virtual void update(const sf::Time& elapsed);
    /**
     * Render entity's sprite.
     * 
     * \param elapsed Ellapsed time from last render.
     * \param window RenderWindow used for rendering.
     */
    virtual void render(const sf::Time& elapsed, sf::RenderWindow& window);
    /**
     * Respawn an entitiy.
     */
    virtual void respawn() {};
    /**
     * Get rigid body of an entity.
     * 
     */
    virtual Rectf getRigid() const;
    /**
     * Get score which will be obtained by player when entitiy is destroyed.
     * 
     */
    virtual int getScore() const { return 0; }
    /**
     * Happens when entitiy die.
     */
    virtual void onDead() {};

    /**
     * Get id of entitiy.
     * 
     */
    int getId() const { return id_; }
    /**
     * Get type of entitiy.
     * 
     */
    EntityType getType() const { return type_; }
    /**
     * Set life bar wich will be associated with entitiy.
     * 
     */
    void setLifeBar(std::shared_ptr<LifeBar> lifeBar) { lifeBar_ = lifeBar; }
    /**
     * Get life bar which is associated with entitiy of nullptr.
     * 
     */
    std::shared_ptr<LifeBar> getLifeBar() const { return lifeBar_; }
protected:
    /**
     * Vector which represent a movement of entitiy.
     */
    sf::Vector2f speed_{ 0, 0 };

    /**
     * Get owner of entity.
     * 
     */
    GameState* getGameState() const { return state_; }
    /**
     * Get sprite of entity.
     * 
     */
    const IRenderPtr getSprite() const { return sprite_; }
private:
    /**
     * Id of entity.
     */
    const unsigned int id_ { lastId_++ };

    /**
     * Owner of entitiy.
     */
    GameState* state_;
    /**
     * Type of entity.
     */
    EntityType type_;
    /**
     * Sprite of entity.
     */
    IRenderPtr sprite_;
    /**
     * Life bar associated with entitiy.
     */
    std::shared_ptr<LifeBar> lifeBar_{ nullptr };

    /**
     * Used for generating entitiy ID.
     */
    static unsigned int lastId_;
};

using EntityPtr = std::shared_ptr<Entity>;

#endif // !ENTITY_HPP