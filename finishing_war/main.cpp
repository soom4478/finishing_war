#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "fish.cpp"

using namespace sf;

int main()
{
    RenderWindow window(VideoMode(1200, 750), "Fishing Game");

    // 배경 텍스처 설정
    Texture startBackgroundTexture, gameBackgroundTexture;
    if (!startBackgroundTexture.loadFromFile("images/background_1.png")) {
        std::cerr << "Failed to load start background image!" << std::endl;
        return -1;
    }
    if (!gameBackgroundTexture.loadFromFile("images/background_2.png")) {
        std::cerr << "Failed to load game background image!" << std::endl;
        return -1;
    }
    Sprite startBackground(startBackgroundTexture);
    Sprite gameBackground(gameBackgroundTexture);

    // 물고기 객체 생성
    Fish myFish(230);
    myFish.setPosition(0.0f, 500.0f);

    float fishSpeed = 0.05f;
    bool movingRight = true;

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    CircleShape player(40.0f);
    player.setFillColor(Color::Green);
    player.setPosition(560.0f, .0f);

    VertexArray line(Lines, 2);
    line[0].color = Color::Black;
    line[1].color = Color::Black;

    float lineSpeed = 0.5f;
    Vector2f targetPosition;
    bool lineMoving = false;
    bool collisionDetected = false;
    bool isFishing = true;
    bool fishCaught = false;
    int clickCount = 0;

    Font font1, font2;
    if (!font1.loadFromFile("fonts/런드리고딕OTF Bold.otf")) {
        return -1;
    }
    if (!font2.loadFromFile("fonts/전남교육유나체 TTF Regular.ttf")) {
        return -1;
    }

    Text startT_1("Fi", font1, 100);
    startT_1.setFillColor(Color::White);
    startT_1.setPosition(425.f, 243.f);
    Text startT_2("ni", font1, 70);
    startT_2.setFillColor(Color::White);
    startT_2.setPosition(520.f, 275.f);
    Text startT_3("sing", font1, 100);
    startT_3.setFillColor(Color::White);
    startT_3.setPosition(580.f, 243.f);
    Text startT_4("WAR", font1, 128);
    startT_4.setFillColor(Color::White);
    startT_4.setPosition(450.f, 320.f);
    Text startT_5("click the screen!", font2, 50);
    startT_5.setFillColor(Color::White);
    startT_5.setPosition(465.f, 450.f);

    int gameState = 0;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (gameState == 0 && event.type == Event::MouseButtonPressed) {
                gameState = 1;
                movingRight = (std::rand() % 2 == 0);
                myFish.setDirection(movingRight);
                myFish.setPosition(movingRight ? 3.0f : 1170.0f, 500.0f);
            }
            else if (gameState == 1 && event.type == Event::MouseButtonPressed && !lineMoving && isFishing) {
                Vector2f playerTopRight = player.getPosition() + Vector2f(player.getRadius(), -player.getRadius());
                line[0].position = playerTopRight;
                line[1].position = playerTopRight;
                targetPosition = Vector2f(event.mouseButton.x, event.mouseButton.y);
                lineMoving = true;
                collisionDetected = false;
                isFishing = false;
            }
            else if (collisionDetected && !lineMoving && clickCount < 10) {
                if (event.type == Event::MouseButtonPressed) {
                    clickCount++;
                    std::cout << "Click count: " << clickCount << "/10" << std::endl;
                }
            }
        }

        window.clear();

        if (gameState == 0) {
            window.draw(startBackground);
            window.draw(startT_1);
            window.draw(startT_2);
            window.draw(startT_3);
            window.draw(startT_4);
            window.draw(startT_5);
        }
        else if (gameState == 1) {
            window.draw(gameBackground);

            if (!collisionDetected) {
                myFish.update();
            }

            if (lineMoving) {
                Vector2f currentPos = line[1].position;

                if (currentPos != targetPosition) {
                    Vector2f direction = targetPosition - currentPos;
                    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                    if (length < lineSpeed) {
                        line[1].position = targetPosition;
                        lineMoving = false;
                        if (!collisionDetected) {
                            std::cout << "Target missed! Resetting the line." << std::endl;
                            isFishing = true;
                            clickCount = 0;
                        }
                    }
                    else {
                        direction /= length;
                        line[1].position += direction * lineSpeed;
                    }
                }
            }

            if (!lineMoving && !collisionDetected) {
                FloatRect fishBounds = myFish.getGlobalBounds();
                Vector2f lineEnd = line[1].position;
                Vector2f lineStart = line[0].position;

                // 물고기와 줄 끝이 충돌했는지 확인
                if (fishBounds.contains(lineEnd)) {
                    std::cout << "Collision detected: Fish touched the line!" << std::endl;
                    collisionDetected = true;
                    fishCaught = true;
                    isFishing = false;
                }
                else {
                    // 줄의 길이를 줄여서 자연스럽게 위로 올라가게 만들기
                    Vector2f lineVector = lineEnd - lineStart;
                    float lineLength = std::sqrt(lineVector.x * lineVector.x + lineVector.y * lineVector.y);

                    if (lineLength > lineSpeed) {
                        Vector2f normalizedVector = lineVector / lineLength;
                        line[1].position -= normalizedVector * lineSpeed;  // 줄 끝을 위로 이동
                    }
                    else {
                        // 줄이 끝에 다다르면 줄을 다시 내려놓고 낚시를 재개
                        isFishing = true;
                        clickCount = 0;
                    }
                }
            }


            if (collisionDetected && !lineMoving && clickCount >= 10) {
                Vector2f start = line[0].position;
                Vector2f end = line[1].position;
                Vector2f lineVector = end - start;

                float length = std::sqrt(lineVector.x * lineVector.x + lineVector.y * lineVector.y);

                if (length > lineSpeed) {
                    Vector2f normalizedVector = lineVector / length;
                    line[1].position -= normalizedVector * lineSpeed;

                    FloatRect fishBounds = myFish.getGlobalBounds();
                    Vector2f fishSize(fishBounds.width, fishBounds.height);
                    myFish.setPosition(line[1].position.x - fishSize.x / 2, line[1].position.y);
                }

                if (length <= lineSpeed) {
                    myFish.printInfo();
                    clickCount = 0;

                    movingRight = (std::rand() % 2 == 0);
                    myFish = Fish(230);
                    myFish.setDirection(movingRight);
                    myFish.setPosition(movingRight ? 3.0f : 1170.0f, 500.0f);

                    lineMoving = false;
                    collisionDetected = false;
                    fishCaught = false;
                    isFishing = true;
                }
            }

            window.draw(line);
            window.draw(player);
            window.draw(myFish);
        }

        window.display();
    }

    return 0;
}
