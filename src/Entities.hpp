#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include "Entity.hpp"

/**
 * Represent a destroyable block entity.
 */
class Block final : public Entity
{
public:
    Block(GameState* state, IRenderPtr sprite)
        : Entity {state, sprite, EntityType::Structure} {}
};

/**
 * Represent an enemy entity.
 */
class Enemy final : public Entity
{
public:
    /**
     * \param distance Distance which has to be flyed by the enemy, before enemy should move down.
     */
    Enemy(GameState* state, std::shared_ptr<ImageSprite> sprite, float distance)
        : Entity {state, sprite, EntityType::Enemy}, maxSteps_((int)(distance / distance_)), sprite_(sprite) {}

    void update(const sf::Time& elapsed) override;
    int getScore() const override { return 10; }
    void onDead() override;
private:
    /**
     * Determine if entity should move this tick.
     */
    bool move_ { false };
    /**
     * Total ellapsed time from last move in ms.
     */
    float total_ { 0.f };
    /**
     * Minimal amount of time delay before entity next move in ms.
     */
    float needed_ { 700.f };
    /**
     * How many pixels enemy move.
     */
    float distance_ { 15.f };
    /**
     * Maximum number of steps in row.
     * After this number is exceeded, enemy will move to the next row.
     */
    int maxSteps_;
    /**
     * Current number of steps, counted from last horizontal move.
     */
    int steps_ { 0 };
    /**
     * Determine if enemy should move down this tick.
     */
    bool down_{ false };
    /**
     * Probability that enemyt will fire a projectile during this tick.
     */
    float fireChance_ { .0001f};
    /**
     * Acceleration of enemy movement.
     */
    float timeAcc_ { 1.5f };
    /**
     * Acceleration of fire chance.
     */
    float fireAcc_ { .00001f };
    std::shared_ptr<ImageSprite> sprite_;
};

/**
 * Represent a projectile entitiy.
 */
class Projectile final : public Entity
{
public:
    /**
     * \param speed Speed of the projectile.
     * \param target Types of entites who are target of projectile.
     */
    Projectile(GameState* state, IRenderPtr sprite, float speed, EntityType target)
        : Entity {state, sprite, EntityType::None}, target_(target)
    {
        speed_ = sf::Vector2f(0.f, speed);
    }

    void update(const sf::Time& elapsed) override;
private:
    EntityType target_;
};

/**
 * Representa life bar entitiy.
 */
class LifeBar final : public Entity
{
public:
    /**
     * \param count Number of lives.
     */
    LifeBar(GameState* state, IRenderPtr sprite, int count)
        : Entity{state, sprite, EntityType::UI}, count_(count) {}

    void render(const sf::Time& elapsed, sf::RenderWindow& window) override;

    /**
     * Decrease number of lives by one.
     * 
     */
    void decreseCount() { --count_; }
    /**
     * Get current number of lives.
     * 
     * \return 
     */
    int getCount() { return count_; }
private:
    /**
     * CUrrent number of lives.
     */
    int count_;
};

using LBarPtr = std::shared_ptr<LifeBar>;

/**
 * Represent an entity controlled by a player..
 */
class Player final : public Entity, std::enable_shared_from_this<Player>
{
public:
    Player(GameState* state, IRenderPtr sprite)
        : Entity{state, sprite, EntityType::Player} {}

    void update(const sf::Time& elapsed) override;
    void respawn() override;
    void onDead() override;
private:
    /**
     * Ellapsed time from player's last shooting action.
     */
    float total_ { 0.f };
    /**
     * Minimal amount of time befere player can shoot again (player can shoot each needed_ ms).
     */
    float needed_ { 800.f };
    /**
     * Determine if player can make a shooting action (fire a projectile).
     */
    bool charge_ { false };
};

/**
 * Represent an ufo entity.
 */
class Ufo final : public Entity
{
public:
    /**
     * \param speed Movement speed of ufo.
     */
    Ufo(GameState* state, IRenderPtr sprite, float speed)
        : Entity{state, sprite, EntityType::Enemy}
    {
         spawned = true;
        speed_ = sf::Vector2f(speed, 0.f);
    }

    void update(const sf::Time& elapsed) override;  
    int getScore() const override { return 50; }

    /**
     * Determine if ufo is currently spawned.
     */
    static bool spawned;
};

/**
 * Represent a socre bar entity.
 */
class ScoreBar final : public Entity
{
public:
    ScoreBar(GameState* state, TextPtr text)
        : Entity{ state, text, EntityType::UI }, text_(text)
    {
        setScore(0);
    }

    /**
     * Get current score on score bar.
     * 
     */
    int getScore() { return score_; }
    /**
     * Set new score to score bar.
     * 
     */
    void setScore(int score)
    {
        score_ = score;
        std::string string = std::to_string(score_);
        while (string.length() < 4)
            string = '0' + string;
        text_->setString(string);
    }
    /**
     * Add score to a current score.
     * 
     */
    void addScore(int score) { setScore(score_ + score); }

private:
    TextPtr text_;
    /**
     * Current score.
     */
    int score_;
};

#endif // !ENTITIES_HPP