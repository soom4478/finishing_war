#include <SFML/Graphics.hpp>

using namespace sf;

void updateFishMovement(RectangleShape& fish, float fishSpeed, bool& movingRight, Texture& fishTextureLeft, Texture& fishTextureRight)
{
    if (movingRight) {
        fish.move(fishSpeed, 0.0f);
        if (fish.getPosition().x >= 1200) {
            movingRight = false;
            fish.setTexture(&fishTextureRight);  // 오른쪽 텍스처로 변경
            fish.setPosition(1200.0f, fish.getPosition().y);
        }
    }
    else {
        fish.move(-fishSpeed, 0.0f);
        if (fish.getPosition().x <= -230) {
            movingRight = true;
            fish.setTexture(&fishTextureLeft);  // 왼쪽 텍스처로 변경
            fish.setPosition(-1.0f, fish.getPosition().y);
        }
    }
}

void drawStartScreen(RenderWindow& window, Text& startT_1, Text& startT_2, Text& startT_3, Text& startT_4, Text& startT_5)
{
    window.draw(startT_1);
    window.draw(startT_2);
    window.draw(startT_3);
    window.draw(startT_4);
    window.draw(startT_5);
}

void drawMainScreen(RenderWindow& window, RectangleShape& fish, CircleShape& player)
{
    window.draw(fish);
    window.draw(player);
}
