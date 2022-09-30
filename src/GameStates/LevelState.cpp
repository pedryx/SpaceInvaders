#include "LevelState.hpp"
#include "../Entities.hpp"
#include "../Game.hpp"

#include <ios>
#include <memory>

void LevelState::init()
{
    // Load sprite sheet which contains textures for all entites.
    TextureManager::getInstance().load("Content/Textures/invaders.png", "invaders");

    generatePlayer();
    generateCluster();
    generateHouses();
}

void LevelState::update(const sf::Time& elapsed)
{
    GameState::update(elapsed);

    elapsed_ += elapsed.asMilliseconds();
    if (elapsed_ >= needed_)
    {
        elapsed_ -= needed_;
        spawnUfo();
    }
}

void LevelState::spawnUfo()
{
    if (Ufo::spawned)
        return;
    
    sf::Vector2f pos(800.f, 70.f);
    sf::Vector2f size(45.f, 20.f);

    std::vector<sf::IntRect> frames;
    frames.emplace_back(30, 338, 192, 84);

    IRenderPtr sprite = std::make_shared<ImageSprite>(pos, size, "invaders", frames);
    EntityPtr ufo = std::make_shared<Ufo>(this, sprite, -.2f);

    addEntity(ufo);
}

void LevelState::generatePlayer()
{
    // create player
    sf::Vector2f pos(400.f, 570.f);
    sf::Vector2f size(60.f, 18.f);

    IRenderPtr sprite = std::make_shared<RectSprite>(pos, size, sf::Color::Green);
    EntityPtr player = std::make_shared<Player>(this, sprite);

    // create life bar
    pos = sf::Vector2f(660.f, 20.f);
    size = size * .7f;

    IRenderPtr spriteLife = std::make_shared<RectSprite>(pos, size, sf::Color::Green);
    LBarPtr lifeBar = std::make_shared<LifeBar>(this, spriteLife, 3);
    player->setLifeBar(lifeBar);

    // create score bar
    sf::Font font;
    if (!font.loadFromFile("Content/Fonts/ARCADE.TTF"))
        throw std::ios_base::failure("Cannot load font!");
    TextPtr textSprite = std::make_shared<TextSprite>(sf::Vector2f(5.f, 0.f), font, "", 60, sf::Color::Yellow);
    scoreBar_ = std::make_shared<ScoreBar>(this, textSprite);

    // add entities
    addEntity(player);
    addEntity(lifeBar);
    addEntity(scoreBar_);
}

void LevelState::generateCluster()
{
    // constants 
    constexpr int countX = 11, countY = 5;
    constexpr float startY = 100.f;
    constexpr  float enemySize = 40.f;
    constexpr float distance = 800.f - countX * enemySize;
    constexpr float sizeScale = .7f;

    // init frames for animation
    std::vector<sf::IntRect> enemyType1;
    enemyType1.emplace_back(40, 30, 64, 64);
    enemyType1.emplace_back(142, 31, 64, 64);

    std::vector<sf::IntRect> enemyType2;
    enemyType2.emplace_back(27, 134, 88, 64);
    enemyType2.emplace_back(131, 134, 88, 64);

    std::vector<sf::IntRect> enemyType3;
    enemyType3.emplace_back(14, 241, 96, 64);
    enemyType3.emplace_back(128, 242, 96, 64);

    // create grid of enemy entities (alien cluster)
    for (int x = 0; x < countX; x++)
    {
        for (int y = 0; y < countY; y++)
        {
            sf::Vector2f pos(enemySize / 2 + x * enemySize, startY + y * enemySize);
            sf::Vector2f size(enemySize * sizeScale, enemySize * sizeScale);

            std::shared_ptr<ImageSprite> sprite = std::make_shared<ImageSprite>(pos, size, "invaders", (y == 0 ? enemyType1 : (y >= 3 ? enemyType3 : enemyType2)));
            addEntity(std::make_shared<Enemy>(this, sprite, distance));
        }
    }
    clusterSize_ = countX * countY;
}

void LevelState::generateHouses()
{
    // constants
    constexpr float startX = 50.f;
    constexpr float startY = 480.f;
    constexpr int houseCount = 4;
    constexpr float width = 800.f / houseCount;

    // generate houseCouns * house
    for (int i = 0; i < houseCount; i++)
        generateHouse(startX + i * width, startY);
}

void LevelState::generateHouse(float startX, float startY)
{
    // constants
    constexpr float blockSize = 10.f;
    constexpr float roofWidth = 100.f;
    constexpr float roofHeight = 30.f;
    constexpr float wallWidth = 20.f;
    constexpr float wallHeight = 30.f;

    // create roof
    generateRectangle(startX, startY, roofWidth, roofHeight, blockSize);

    // create walls
    generateRectangle(startX, startY + roofHeight, wallWidth, wallHeight, blockSize);
    generateRectangle(startX + roofWidth - wallWidth, startY + roofHeight, wallWidth,
     wallHeight, blockSize);
}

void LevelState::generateRectangle(float startX, float startY, float width, float height,
 float blockSize)
{
    for (int y = 0; y < (int)(height / blockSize); y++)
    {
        for (int x = 0; x < (int)(width / blockSize); x++)
        {
            sf::Vector2f pos(startX + x * blockSize, startY + y * blockSize);
            sf::Vector2f size(blockSize, blockSize);

            IRenderPtr sprite = std::make_shared<RectSprite>(pos, size, sf::Color::Green);
            addEntity(std::make_shared<Block>(this, sprite));
        }
    }
}