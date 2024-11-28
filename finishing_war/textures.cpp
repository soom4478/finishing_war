#include <SFML/Graphics.hpp>

using namespace sf;

Texture loadFishTextureLeft()
{
    Texture fishTextureLeft;
    if (!fishTextureLeft.loadFromFile("images/fish1_1.png")) {
        throw std::runtime_error("Failed to load fish texture left.");
    }
    return fishTextureLeft;
}

Texture loadFishTextureRight()
{
    Texture fishTextureRight;
    if (!fishTextureRight.loadFromFile("images/fish1_2.png")) {
        throw std::runtime_error("Failed to load fish texture right.");
    }
    return fishTextureRight;
}
