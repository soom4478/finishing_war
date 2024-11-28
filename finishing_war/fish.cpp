#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

using namespace sf;
using namespace std;

class Fish {
private:
    RectangleShape fishShape;
    Texture fishTextureLeft;
    Texture fishTextureRight;
    float speed;

public:
    string name;
    int size;
    string value;
    string area;
    int coin;

    bool movingRight;

    Fish(int sizeInput) : size(sizeInput), movingRight(true), speed(0.05f) {
        name = "Goldfish";
        value = "Rare";
        area = "River";
        coin = 100;

        // 텍스처 로드
        if (!fishTextureRight.loadFromFile("images/fish1_1.png") ||
            !fishTextureLeft.loadFromFile("images/fish1_2.png")) {
            throw runtime_error("Failed to load fish textures");
        }

        // 물고기 모양 초기화
        fishShape.setSize(Vector2f(static_cast<float>(size), static_cast<float>(size / 2)));
        fishShape.setTexture(&fishTextureRight);
        fishShape.setPosition(-1.0f, 500.0f);  // 초기 위치 설정
    }

    void setPosition(float x, float y) {
        fishShape.setPosition(x, y);
    }

    void setPosition(const Vector2f& position) {
        fishShape.setPosition(position);
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

    void setDirection(bool movingRight) {
        fishShape.setTexture(movingRight ? &fishTextureRight : &fishTextureLeft);
        this->movingRight = movingRight;
    }

    FloatRect getGlobalBounds() const {
        return fishShape.getGlobalBounds();
    }

    void setSpeed(float newSpeed) {
        speed = newSpeed;
    }

    void update() {
        if (speed == 0.0f) {
            return;
        }

        if (movingRight) {
            move(speed, 0.0f);
        }
        else {
            move(-speed, 0.0f);
        }

        // 화면을 벗어나지 않도록 이동 방향을 반전시킴
        if (getPosition().x >= 1200 || getPosition().x <= -230) {
            movingRight = !movingRight;
            setDirection(movingRight);
        }
    }

    void printInfo() const {
        cout << "Fish Info:" << endl;
        cout << "Name: " << name << endl;
        cout << "Size: " << size << endl;
        cout << "Value: " << value << endl;
        cout << "Area: " << area << endl;
        cout << "Coin: " << coin << endl;
    }

    // 물고기를 화면에 그리기 위한 draw 메서드 추가
    void draw(RenderWindow& window) {
        window.draw(fishShape);  // 올바른 매개변수 전달
    }
};