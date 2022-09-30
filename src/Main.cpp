#include "Game.hpp"
#include "GameStates/LevelState.hpp"

#include <memory>
#include <cstdlib>
#include <ctime>

#include <iostream>

int main()
{
    // create seed used for generation of random numbers
    srand((unsigned)time(0));

    // create game and game state
    Game game(sf::Vector2u(800, 600), "Space Invaders");
    GameStatePtr state = std::make_shared<LevelState>(&game);

    // set game state and launch the game
    game.setState(state);
    game.run();
    
    return 0;
}