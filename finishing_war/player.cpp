#include <SFML/Graphics.hpp>

using namespace sf;

CircleShape createPlayer()
{
    CircleShape player(40.0f);
    player.setFillColor(Color::Green);
    player.setPosition(560.0f, 50.0f);
    return player;
}
