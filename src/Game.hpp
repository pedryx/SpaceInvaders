#ifndef GAME_HPP
#define GAME_HPP

#include "GameState.hpp"

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

/**
 * Represents a game.
 */
class Game
{
public:
    /**
     * Create game.
     * 
     * \param windowSize Size of window.
     * \param title Window's title.
     */
    Game(sf::Vector2u windowSize, const std::string title) 
        : window_(sf::VideoMode(windowSize.x, windowSize.y), title) {}

    /**
     * Launch the game loop.
     * 
     */
    void run();
    /**
     * Set current game state.
     * 
     * \param state Pointer to a new game state.
     */
    void setState(GameStatePtr state);

    /**
     * Get window's size.
     * 
     */
    sf::Vector2u getWinSize() const { return window_.getSize(); }
    /**
     * Exit the game.
     * 
     */
    void exit();
private:
    sf::RenderWindow window_;
    /**
     * Active game state.
     */
    GameStatePtr state_;
    /**
     * Determine if game should exit after this iteration.
     */
    bool exit_{ false };

    /**
     * Initialize the game.
     * 
     */
    void init();
    /**
     * Update game's state.
     * 
     * \param elapsed Ellapsed time from last update.
     */
    void update(const sf::Time& elapsed);
    /**
     * Render the game.
     * 
     * \param elapsed Ellapsed time form last render.
     */
    void render(const sf::Time& elapsed);
};

#endif