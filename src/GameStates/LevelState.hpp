#ifndef LEVELSTATE_HPP
#define LEVELSTATE_HPP

#include "../GameState.hpp"

/**
 * Represent a level game state.
 */
class LevelState final : public GameState
{
public:
    LevelState(Game* game) : GameState {game} {}

    void init() override;
    void update(const sf::Time& elapsed) override;
private:
    friend class Enemy;
    /**
     * Ellapsed time in ms from last ufo spawn.
     */
    float elapsed_{ 0 };
    /**
     * Time needed in ms for ufo to be spawned (ufo will spawn each needed_ ms).
     */
    float needed_ { 30000.f };
    /**
     * Number of enemis in alien cluster.
     */
    int clusterSize_{ 0 };

    /**
     * Spawn ufo.
     * 
     */
    void spawnUfo();
    /**
     * Create player entity and his life and score bar.
     * 
     */
    void generatePlayer();
    /**
     * Create cluster of aliens.
     * 
     */
    void generateCluster();
    /**
     * Create houses.
     * 
     */
    void generateHouses();
    /**
     * Create one house at specific position.
     * 
     * \param startX X coor of top-left corner of house location.
     * \param startY Y coor of top-left corner of house location.
     */
    void generateHouse(float startX, float startY);
    /**
     * Create rectangle out of Block entities.
     * 
     * \param startX X coor of top-left corner of rectangle location.
     * \param startY Y coor of top-left corner of rectangle location.
     * \param width Width of the rectangle.
     * \param height Height of the rectangle.
     * \param blockSize Size of each block entitiy.
     */
    void generateRectangle(float startX, float startY, float width, float height,
     float blockSize);
};

#endif // !LEVELSTATE_HPP