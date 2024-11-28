#include <SFML/Graphics.hpp>
#include <cstdlib> // ���� ������
#include <ctime>   // ���� �õ��
#include <iostream> // �ܼ� ��¿�

using namespace sf;

int main()
{
    RenderWindow window(VideoMode(1200, 750), "Fishing Game");

    // ���� ����
    Color backgroundColor = Color(135, 206, 250); // �ϴû� (RGB)

    // ����� �ؽ�ó �� ��ü ����
    Texture fishTextureLeft, fishTextureRight;
    if (!fishTextureLeft.loadFromFile("images/fish1_1.png") ||
        !fishTextureRight.loadFromFile("images/fish1_2.png")) {
        return -1; // �̹��� �ε� ���� �� ���α׷� ����
    }

    RectangleShape fish(Vector2f(230.f, 123.f));
    fish.setTexture(&fishTextureRight);
    fish.setPosition(-1.0f, 500.0f);

    // ����� �̵� ���� ����
    float fishSpeed = 0.05f; // ����� �̵� �ӵ�
    bool movingRight = true;
    bool fishCaught = false; // ����� ���� ����

    // ���� �õ� �ʱ�ȭ
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // �÷��̾�
    CircleShape player(40.0f);
    player.setFillColor(Color::Green);
    player.setPosition(560.0f, 50.0f);

    // ������ ����
    VertexArray line(Lines, 2);
    line[0].color = Color::Black;
    line[1].color = Color::Black;

    float lineSpeed = 0.5f;         // ������ �̵� �ӵ� (õõ�� �پ��)
    Vector2f targetPosition;       // ������ ��ǥ ��ǥ
    bool lineMoving = false;       // �������� �����̴� ������ ����
    bool collisionDetected = false; // �浹 ���� �÷���
    int clickCount = 0;            // Ŭ�� Ƚ��
    bool isFishing = true;         // ���� ���� ����

    // ���� ȭ�� �ؽ�Ʈ
    Font font1, font2;
    if (!font1.loadFromFile("fonts/���帮���OTF Bold.otf")) {
        return -1;
    }
    if (!font2.loadFromFile("fonts/������������ü TTF Regular.ttf")) {
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

    // ���� ���� ���� (0: ���� ȭ��, 1: ���� ȭ��)
    int gameState = 0;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            // ���� ȭ�鿡�� Ŭ���ϸ� ���� ȭ������ ��ȯ
            if (gameState == 0 && event.type == Event::MouseButtonPressed) {
                gameState = 1;
                movingRight = (std::rand() % 2 == 0);
                fish.setTexture(movingRight ? &fishTextureLeft : &fishTextureRight);
                float startX = movingRight ? 3.0f : 1170.0f;
                fish.setPosition(startX, 500.0f);
            }
            else if (gameState == 1 && event.type == Event::MouseButtonPressed && !lineMoving && isFishing) {
                // ������ ���� ��ġ �� ��ǥ ��ġ ����
                Vector2f playerTopRight = player.getPosition() + Vector2f(player.getRadius(), -player.getRadius());
                line[0].position = playerTopRight;
                line[1].position = playerTopRight; // �ʱ⿡�� ������ �������� ����
                targetPosition = Vector2f(event.mouseButton.x, event.mouseButton.y); // Ŭ���� ��ġ�� ��ǥ�� ����
                lineMoving = true; // ������ �̵� ����
                collisionDetected = false; // �浹 ���� �ʱ�ȭ
                fishCaught = false; // ����� ���� �ʱ�ȭ
                clickCount = 0; // Ŭ�� Ƚ�� �ʱ�ȭ
            }
            else if (gameState == 1 && event.type == Event::MouseButtonPressed && collisionDetected) {
                // �浹 �� Ŭ�� Ƚ�� ����
                clickCount++;
            }
        }

        // ȭ�� ������
        window.clear(backgroundColor);

        if (gameState == 0) {
            // ���� ȭ��
            window.draw(startT_1);
            window.draw(startT_2);
            window.draw(startT_3);
            window.draw(startT_4);
            window.draw(startT_5);
        }
        else if (gameState == 1) {
            // ����� �̵�
            if (!fishCaught) {
                if (movingRight) {
                    fish.move(fishSpeed, 0.0f);
                    if (fish.getPosition().x >= 1200) {
                        movingRight = false;
                        fish.setTexture(&fishTextureRight);
                        fish.setPosition(1200.0f, fish.getPosition().y);
                    }
                }
                else {
                    fish.move(-fishSpeed, 0.0f);
                    if (fish.getPosition().x <= -230) {
                        movingRight = true;
                        fish.setTexture(&fishTextureLeft);
                        fish.setPosition(-1.0f, fish.getPosition().y);
                    }
                }
            }

            // ������ �̵�
            if (lineMoving) {
                Vector2f currentPos = line[1].position;

                // �������� ��ǥ�� ������ ������ �̵�
                if (currentPos != targetPosition) {
                    Vector2f direction = targetPosition - currentPos;
                    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                    if (length < lineSpeed) {
                        line[1].position = targetPosition; // ��ǥ ����
                        lineMoving = false;
                    }
                    else {
                        direction /= length; // ���� ���� ����ȭ
                        line[1].position += direction * lineSpeed; // �� ���� ���� �Ÿ� �̵�
                    }
                }
            }

            // �浹 ����
            if (!lineMoving && !collisionDetected) {
                FloatRect fishBounds = fish.getGlobalBounds();
                Vector2f lineEnd = line[1].position;

                if (fishBounds.contains(lineEnd)) {
                    std::cout << "Collision detected: Fish touched the line!" << std::endl;
                    collisionDetected = true;
                    fishCaught = true;
                    isFishing = false; // ���� ���� ���
                }
            }

            if (collisionDetected && clickCount >= 10) {
                // �������� �������� ����
                Vector2f start = line[0].position;
                Vector2f end = line[1].position;

                // �������� ���� ���� (���� - ������)
                Vector2f lineVector = end - start;

                // �������� ���̸� ���̱� ���� ����ȭ ���� ���
                float length = std::sqrt(lineVector.x * lineVector.x + lineVector.y * lineVector.y);
                if (length > lineSpeed) {
                    Vector2f normalizedVector = lineVector / length; // ���� ����ȭ

                    // ������ ������ ������ ������ �̵� (���� ����)
                    line[1].position -= normalizedVector * lineSpeed;

                    // ����� ��ġ�� ������ ���� ����
                    fish.setPosition(line[1].position.x - fish.getSize().x / 2, line[1].position.y);
                }

                // ������ ���̰� ����� �پ������� ���� ���� ���·� ��ȯ
                if (length <= lineSpeed) {
                    isFishing = true;
                }
            }

            // ���� ȭ��
            window.draw(fish);
            window.draw(player);
            window.draw(line); // ������ �׸���
        }

        window.display();
    }

    return 0;
}
