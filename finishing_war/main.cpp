#include <SFML/Graphics.hpp>
#include <cstdlib>  // ���� ������
#include <ctime>    // ���� �õ��
#include <iostream> // �ܼ� ��¿�
#include "fish.cpp"  // Fish Ŭ���� ���� ���� ����

using namespace sf;

int main()
{
    RenderWindow window(VideoMode(1200, 750), "Fishing Game");

    // ���� ����
    Color backgroundColor = Color(135, 206, 250); // �ϴû� (RGB)

    // ����� ��ü ���� (ũ�� 230)
    Fish myFish(230);
    myFish.setPosition(0.0f, 500.0f); // �ʱ� ��ġ ����

    // ����� �̵� ���� ����
    float fishSpeed = 0.05f; // ����� �̵� �ӵ�
    bool movingRight = true; // ����� �̵� ����

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
    bool isFishing = true;         // ���� ���� ����
    bool fishCaught = false;       // ����� ���� ����
    int clickCount = 0;            // Ŭ�� Ƚ��

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
                myFish.setDirection(movingRight); // ���� ���� �Լ� ȣ��
                myFish.setPosition(movingRight ? 3.0f : 1170.0f, 500.0f);
            }
            else if (gameState == 1 && event.type == Event::MouseButtonPressed && !lineMoving && isFishing) {
                // ������ ���� ��ġ �� ��ǥ ��ġ ����
                Vector2f playerTopRight = player.getPosition() + Vector2f(player.getRadius(), -player.getRadius());
                line[0].position = playerTopRight;
                line[1].position = playerTopRight; // �ʱ⿡�� ������ �������� ����
                targetPosition = Vector2f(event.mouseButton.x, event.mouseButton.y); // Ŭ���� ��ġ�� ��ǥ�� ����
                lineMoving = true; // ������ �̵� ����
                collisionDetected = false; // �浹 ���� �ʱ�ȭ
                isFishing = false; // ���� ���� ���
            }
            // �����ٿ� ����Ⱑ ������ Ŭ���� 10�� �ؾ� �پ��
            else if (collisionDetected && !lineMoving && clickCount < 10) {
                if (event.type == Event::MouseButtonPressed) {
                    clickCount++;
                    std::cout << "Click count: " << clickCount << "/10" << std::endl;
                }
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
            if (!collisionDetected) {
                myFish.update(); // ����� ������Ʈ (�浹�� ������ ��� �̵�)
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
                        line[1].position += direction * lineSpeed; // ������ �̵�
                    }
                }
            }

            // �浹 ����
            if (!lineMoving && !collisionDetected) {
                FloatRect fishBounds = myFish.getGlobalBounds();
                Vector2f lineEnd = line[1].position;

                // ������ ���� ����� ���� �ȿ� �ִ��� Ȯ��
                if (fishBounds.contains(lineEnd)) {
                    std::cout << "Collision detected: Fish touched the line!" << std::endl;
                    collisionDetected = true;
                    fishCaught = true;
                    isFishing = false; // ���� ���� ���
                }
            }

            // ������ ���� ���̱�
            if (collisionDetected && !lineMoving && clickCount >= 10) {
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
                    FloatRect fishBounds = myFish.getGlobalBounds(); // ����� ��� ��������
                    Vector2f fishSize(fishBounds.width, fishBounds.height); // ����� ũ��
                    myFish.setPosition(line[1].position.x - fishSize.x / 2, line[1].position.y);
                }

                // ������ ���̰� ����� �پ������� ���� ���� ���·� ��ȯ
                if (length <= lineSpeed) {
                    isFishing = true;
                    clickCount = 0; // Ŭ�� Ƚ�� �ʱ�ȭ
                }
            }

            // ������ �׸���
            window.draw(line);

            // ����� �׸���
            window.draw(myFish.getShape());  // ����� �׸���
            window.draw(player);
        }

        window.display();
    }

    return 0;
}