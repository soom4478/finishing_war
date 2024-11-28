#include <SFML/Graphics.hpp>
#include <cstdlib>  // 랜덤 생성용
#include <ctime>    // 랜덤 시드용
#include <iostream> // 콘솔 출력용
#include "fish.cpp"  // Fish 클래스 구현 파일 포함

using namespace sf;

int main()
{
    RenderWindow window(VideoMode(1200, 750), "Fishing Game");

    // 배경색 설정
    Color backgroundColor = Color(135, 206, 250); // 하늘색 (RGB)

    // 물고기 객체 생성 (크기 230)
    Fish myFish(230);
    myFish.setPosition(0.0f, 500.0f); // 초기 위치 설정

    // 물고기 이동 관련 설정
    float fishSpeed = 0.05f; // 물고기 이동 속도
    bool movingRight = true; // 물고기 이동 방향

    // 랜덤 시드 초기화
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // 플레이어
    CircleShape player(40.0f);
    player.setFillColor(Color::Green);
    player.setPosition(560.0f, 50.0f);

    // 낚시줄 설정
    VertexArray line(Lines, 2);
    line[0].color = Color::Black;
    line[1].color = Color::Black;

    float lineSpeed = 0.5f;         // 낚시줄 이동 속도 (천천히 줄어듦)
    Vector2f targetPosition;       // 낚시줄 목표 좌표
    bool lineMoving = false;       // 낚시줄이 움직이는 중인지 여부
    bool collisionDetected = false; // 충돌 여부 플래그
    bool isFishing = true;         // 낚시 가능 여부
    bool fishCaught = false;       // 물고기 낚음 여부
    int clickCount = 0;            // 클릭 횟수

    // 시작 화면 텍스트
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

    // 현재 상태 관리 (0: 시작 화면, 1: 메인 화면)
    int gameState = 0;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            // 시작 화면에서 클릭하면 메인 화면으로 전환
            if (gameState == 0 && event.type == Event::MouseButtonPressed) {
                gameState = 1;
                movingRight = (std::rand() % 2 == 0);
                myFish.setDirection(movingRight); // 방향 설정 함수 호출
                myFish.setPosition(movingRight ? 3.0f : 1170.0f, 500.0f);
            }
            else if (gameState == 1 && event.type == Event::MouseButtonPressed && !lineMoving && isFishing) {
                // 낚시줄 시작 위치 및 목표 위치 설정
                Vector2f playerTopRight = player.getPosition() + Vector2f(player.getRadius(), -player.getRadius());
                line[0].position = playerTopRight;
                line[1].position = playerTopRight; // 초기에는 끝점이 시작점과 동일
                targetPosition = Vector2f(event.mouseButton.x, event.mouseButton.y); // 클릭한 위치를 목표로 설정
                lineMoving = true; // 낚시줄 이동 시작
                collisionDetected = false; // 충돌 상태 초기화
                isFishing = false; // 낚시 동작 잠금
            }
            // 낚시줄에 물고기가 닿으면 클릭을 10번 해야 줄어듬
            else if (collisionDetected && !lineMoving && clickCount < 10) {
                if (event.type == Event::MouseButtonPressed) {
                    clickCount++;
                    std::cout << "Click count: " << clickCount << "/10" << std::endl;
                }
            }
        }

        // 화면 렌더링
        window.clear(backgroundColor);

        if (gameState == 0) {
            // 시작 화면
            window.draw(startT_1);
            window.draw(startT_2);
            window.draw(startT_3);
            window.draw(startT_4);
            window.draw(startT_5);
        }
        else if (gameState == 1) {
            // 물고기 이동
            if (!collisionDetected) {
                myFish.update(); // 물고기 업데이트 (충돌이 없으면 계속 이동)
            }

            // 낚시줄 이동
            if (lineMoving) {
                Vector2f currentPos = line[1].position;

                // 낚시줄이 목표에 도달할 때까지 이동
                if (currentPos != targetPosition) {
                    Vector2f direction = targetPosition - currentPos;
                    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                    if (length < lineSpeed) {
                        line[1].position = targetPosition; // 목표 도달
                        lineMoving = false;
                    }
                    else {
                        direction /= length; // 방향 벡터 단위화
                        line[1].position += direction * lineSpeed; // 낚시줄 이동
                    }
                }
            }

            // 충돌 감지
            if (!lineMoving && !collisionDetected) {
                FloatRect fishBounds = myFish.getGlobalBounds();
                Vector2f lineEnd = line[1].position;

                // 낚시줄 끝이 물고기 영역 안에 있는지 확인
                if (fishBounds.contains(lineEnd)) {
                    std::cout << "Collision detected: Fish touched the line!" << std::endl;
                    collisionDetected = true;
                    fishCaught = true;
                    isFishing = false; // 낚시 동작 잠금
                }
            }

            // 낚시줄 길이 줄이기
            if (collisionDetected && !lineMoving && clickCount >= 10) {
                // 낚시줄의 시작점과 끝점
                Vector2f start = line[0].position;
                Vector2f end = line[1].position;

                // 낚시줄의 현재 벡터 (끝점 - 시작점)
                Vector2f lineVector = end - start;

                // 낚시줄의 길이를 줄이기 위한 정규화 벡터 계산
                float length = std::sqrt(lineVector.x * lineVector.x + lineVector.y * lineVector.y);

                if (length > lineSpeed) {
                    Vector2f normalizedVector = lineVector / length; // 벡터 정규화

                    // 낚시줄 끝점을 시작점 쪽으로 이동 (비율 유지)
                    line[1].position -= normalizedVector * lineSpeed;

                    // 물고기 위치를 낚시줄 끝에 맞춤
                    FloatRect fishBounds = myFish.getGlobalBounds(); // 물고기 경계 가져오기
                    Vector2f fishSize(fishBounds.width, fishBounds.height); // 물고기 크기
                    myFish.setPosition(line[1].position.x - fishSize.x / 2, line[1].position.y);
                }

                // 낚시줄 길이가 충분히 줄어들었으면 낚시 가능 상태로 전환
                if (length <= lineSpeed) {
                    isFishing = true;
                    clickCount = 0; // 클릭 횟수 초기화
                }
            }

            // 낚시줄 그리기
            window.draw(line);

            // 물고기 그리기
            window.draw(myFish.getShape());  // 물고기 그리기
            window.draw(player);
        }

        window.display();
    }

    return 0;
}