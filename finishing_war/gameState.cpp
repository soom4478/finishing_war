#include <SFML/Graphics.hpp>
#include <cstdlib>

using namespace sf;

int handleEvents(Event& event, int gameState, bool& movingRight, RectangleShape& fish)
{
    if (gameState == 0 && event.type == Event::MouseButtonPressed) {
        gameState = 1;
        movingRight = (std::rand() % 2 == 0);
        fish.setTexture(movingRight ? fish.getTexture() : fish.getTexture());
        float startX = movingRight ? 3.0f : 1170.0f;
        fish.setPosition(startX, 500.0f);
    }

    return gameState;
}
