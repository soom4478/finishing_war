﻿#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

using namespace sf;
using namespace std;

class Fish : public Drawable {  // sf::Drawable 상속
protected:
    RectangleShape fishShape;
    Texture fishTextureLeft;
    Texture fishTextureRight;
    float speed;

public:
    string name = "fish";
    int size;
    string value = "common";
    string area = "pond";
    int coin = 100;
    string imgPath;

    bool movingRight;

    // 생성자에서 텍스처 파일 경로를 파라미터로 받음
    Fish(int sizeInput, const string& textureRightPath, const string& textureLeftPath)
        : size(sizeInput), movingRight(true), speed(0.05f), imgPath(textureRightPath){

        // 텍스처 로드
        if (!fishTextureRight.loadFromFile(textureRightPath) ||
            !fishTextureLeft.loadFromFile(textureLeftPath)) {
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
        cout << "Name: " << name << endl;
        cout << "Size: " << size << endl;
        cout << "Value: " << value << endl;
        cout << "Area: " << area << endl;
        cout << "Coin: " << coin << endl;
    }

    String getName() const {
        return "common fish";
    }

    int getSize() const {
        return size;
    }

    String getValue() const {
        return value;
    }

    String getArea() const {
        return area;
    }

    int getCoin() const {
        return coin;
    }

    String getImg() const {
        return imgPath;
    }

    // 물고기를 화면에 그리기 위한 draw 메서드 오버라이드
    virtual void draw(RenderTarget& target, RenderStates states) const override {
        target.draw(fishShape, states);  // fishShape를 그립니다
    }
};

class MiniFish : public Fish {
private:
    Texture miniFishTextureLeft;
    Texture miniFishTextureRight;

public:
    // MiniFish 생성자에서 텍스처 파일 경로를 받음
    MiniFish(int sizeInput, const string& textureRightPath, const string& textureLeftPath)
        : Fish(sizeInput, textureRightPath, textureLeftPath) {  // 부모 클래스 생성자 호출
        // MiniFish에 맞는 고유 값들 설정
        name = "mini fish";  // 이름 변경
        value = "Common";   // 가치 변경
        area = "pond";      // 영역 변경
        coin = 50;          // 코인 변경
        setSpeed(0.1f);     // 속도 설정 (다르게 설정 가능)

        // MiniFish의 텍스처 로드
        if (!miniFishTextureRight.loadFromFile(textureRightPath) ||
            !miniFishTextureLeft.loadFromFile(textureLeftPath)) {
            throw runtime_error("Failed to load MiniFish textures");
        }

        // MiniFish의 크기 변경 (기본 크기와 비율을 다르게 설정)
        fishShape.setSize(Vector2f(static_cast<float>(size / 2), static_cast<float>(size / 4)));
        fishShape.setPosition(100.0f, 500.0f);  // 초기 위치 설정
        setDirection(true);  // 초기 방향 설정 (오른쪽으로 이동)
    }

    // MiniFish에 필요한 메서드를 여기에 추가할 수 있습니다.
};
