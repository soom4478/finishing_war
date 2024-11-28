#include <SFML/Graphics.hpp>
#include <string>  // string ����� ���� ���
#include <iostream> // �ܼ� ����� ���� ���

using namespace sf;  // SFML ���� Ŭ���� ���
using namespace std; // string ����� ���� ���ӽ����̽�

class Fish {
private:
    RectangleShape fishShape;  // ����� ���
    Texture fishTextureLeft;   // ���� ���� �ؽ�ó
    Texture fishTextureRight;  // ������ ���� �ؽ�ó
    float speed;               // ����� �ӵ�

public:
    string name;  // ����� �̸�
    int size;     // ����� ũ��
    string value; // ����� ���
    string area;  // ����� ����
    int coin;     // ����� ����

    bool movingRight; // ����� �̵� ����

    // ������: size�� �Ķ���ͷ� �ް�, �������� �⺻������ �ʱ�ȭ
    Fish(int sizeInput) : size(sizeInput), movingRight(true), speed(0.05f) {
        name = "Goldfish";
        value = "Rare";
        area = "River";
        coin = 100;

        // �̹��� �ε�
        if (!fishTextureRight.loadFromFile("images/fish1_1.png") ||
            !fishTextureLeft.loadFromFile("images/fish1_2.png")) {
            throw runtime_error("Failed to load fish textures");
        }

        // ����� ��� �ʱ�ȭ
        fishShape.setSize(Vector2f(static_cast<float>(size), static_cast<float>(size / 2)));
        fishShape.setTexture(&fishTextureRight); // �⺻ �ؽ�ó�� ������
        fishShape.setPosition(-1.0f, 500.0f);    // �ʱ� ��ġ ����
    }

    void setPosition(float x, float y) {
        fishShape.setPosition(x, y);
    }

    Vector2f getPosition() const {
        return fishShape.getPosition();
    }

    RectangleShape& getShape() {
        return fishShape;
    }

    void move(float offsetX, float offsetY) {
        fishShape.move(offsetX, offsetY);
    }

    // �̵� ���⿡ ���� �ؽ�ó ����
    void setDirection(bool movingRight) {
        fishShape.setTexture(movingRight ? &fishTextureRight : &fishTextureLeft);
        this->movingRight = movingRight; // ���� ����
    }

    FloatRect getGlobalBounds() const {
        return fishShape.getGlobalBounds();
    }

    // ����� �ӵ� ����
    void setSpeed(float newSpeed) {
        speed = newSpeed;
    }

    // ����� �̵� (�ӵ� ����)
    void update() {
        if (speed == 0.0f) {
            return; // �ӵ��� 0�̸� �̵����� ����
        }

        if (movingRight) {
            move(speed, 0.0f);
        }
        else {
            move(-speed, 0.0f);
        }

        // ȭ�� ��迡�� �ݻ�
        if (getPosition().x >= 1200 || getPosition().x <= -230) {
            movingRight = !movingRight; // ���� ����
            setDirection(movingRight);
        }
    }

    // ����� ���� ���
    void printInfo() const {
        cout << "Fish Info:" << endl;
        cout << "Name: " << name << endl;
        cout << "Size: " << size << endl;
        cout << "Value: " << value << endl;
        cout << "Area: " << area << endl;
        cout << "Coin: " << coin << endl;
    }
};
