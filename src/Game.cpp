#include "Game.hpp"

void Game::run()
{
    init();

    sf::Clock clock;
    while (window_.isOpen() && !exit_)
    {
        // handle window's events
        sf::Event event;
        while (window_.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window_.close();
                    break;
            }
        }

        sf::Time elapsed = clock.restart();
        update(elapsed);
        render(elapsed);
    }   
}

void Game::setState(GameStatePtr state)
{
    state->init();
    state_ = state;
}

void Game::init()
{
    window_.setFramerateLimit(60);
}

void Game::update(const sf::Time& elapsed)
{
    state_->update(elapsed);
}

void Game::render(const sf::Time& elapsed)
{
    window_.clear(sf::Color::Black);
    state_->render(elapsed, window_);
    window_.display();
}

void Game::exit()
{
    exit_ = true;
}