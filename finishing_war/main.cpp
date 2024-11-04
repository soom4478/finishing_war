#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
    RenderWindow window(VideoMode(1200, 750), "SFML works!");
    
    // 물고기
    RectangleShape fish(Vector2f(100.f, 50.f));
    fish.setFillColor(Color::Blue);
    fish.setPosition(30.0f, 500.0f);

    // 플레이어
    CircleShape player(40.0f);
    player.setFillColor(Color::Green);
    player.setPosition(560.0f, 50.0f);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            fish.move(2.0f, 0.0f);
            if (fish.getPosition().x >= 1200) {
                fish.setPosition(30.0f, fish.getPosition().y);
            }
        }

        window.clear();
        window.draw(fish);
        window.draw(player);
        window.display();
    }

    return 0;
}