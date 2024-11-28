#include <SFML/Graphics.hpp>
#include <string>  // string 사용을 위한 헤더
#include <iostream> // 콘솔 출력을 위한 헤더

using namespace sf;  // SFML 관련 클래스 사용
using namespace std; // string 사용을 위한 네임스페이스

class Fish {
private:
    RectangleShape fishShape;  // 물고기 모양
    Texture fishTextureLeft;   // 왼쪽 방향 텍스처
    Texture fishTextureRight;  // 오른쪽 방향 텍스처
    float speed;               // 물고기 속도

public:
    string name;  // 물고기 이름
    int size;     // 물고기 크기
    string value; // 물고기 등급
    string area;  // 물고기 지역
    int coin;     // 물고기 점수

    bool movingRight; // 물고기 이동 방향

    // 생성자: size는 파라미터로 받고, 나머지는 기본값으로 초기화
    Fish(int sizeInput) : size(sizeInput), movingRight(true), speed(0.05f) {
        name = "Goldfish";
        value = "Rare";
        area = "River";
        coin = 100;

        // 이미지 로드
        if (!fishTextureRight.loadFromFile("images/fish1_1.png") ||
            !fishTextureLeft.loadFromFile("images/fish1_2.png")) {
            throw runtime_error("Failed to load fish textures");
        }

        // 물고기 모양 초기화
        fishShape.setSize(Vector2f(static_cast<float>(size), static_cast<float>(size / 2)));
        fishShape.setTexture(&fishTextureRight); // 기본 텍스처는 오른쪽
        fishShape.setPosition(-1.0f, 500.0f);    // 초기 위치 설정
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

    // 이동 방향에 따른 텍스처 변경
    void setDirection(bool movingRight) {
        fishShape.setTexture(movingRight ? &fishTextureRight : &fishTextureLeft);
        this->movingRight = movingRight; // 방향 설정
    }

    FloatRect getGlobalBounds() const {
        return fishShape.getGlobalBounds();
    }

    // 물고기 속도 설정
    void setSpeed(float newSpeed) {
        speed = newSpeed;
    }

    // 물고기 이동 (속도 적용)
    void update() {
        if (speed == 0.0f) {
            return; // 속도가 0이면 이동하지 않음
        }

        if (movingRight) {
            move(speed, 0.0f);
        }
        else {
            move(-speed, 0.0f);
        }

        // 화면 경계에서 반사
        if (getPosition().x >= 1200 || getPosition().x <= -230) {
            movingRight = !movingRight; // 방향 반전
            setDirection(movingRight);
        }
    }

    // 물고기 정보 출력
    void printInfo() const {
        cout << "Fish Info:" << endl;
        cout << "Name: " << name << endl;
        cout << "Size: " << size << endl;
        cout << "Value: " << value << endl;
        cout << "Area: " << area << endl;
        cout << "Coin: " << coin << endl;
    }
};
