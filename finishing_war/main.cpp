#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "fish.cpp"

using namespace sf;
using namespace std;

int main()
{
    RenderWindow window(VideoMode(1200, 750), "Fishing Game");
    locale::global(std::locale("ko_KR.UTF-8"));

    // 배경 텍스처 설정
    Texture startBackgroundTexture, gameBackgroundTexture, badendingTexture, happyendingTexture;
    Texture clickTexture, infoTexture, btn1Textrue, playerTexture, fishInfoTexture;

    if (!startBackgroundTexture.loadFromFile("images/background_1.png")) {
        cerr << "Failed to load start background image!" << std::endl;
        return -1;
    }
    if (!gameBackgroundTexture.loadFromFile("images/background_2.png")) {
        cerr << "Failed to load game background image!" << std::endl;
        return -1;
    }
    if (!badendingTexture.loadFromFile("images/badending.png")) {
        cerr << "Failed to load game background image!" << std::endl;
        return -1;
    }
    if (!happyendingTexture.loadFromFile("images/happyending.png")) {
        cerr << "Failed to load game background image!" << std::endl;
        return -1;
    }

    if (!clickTexture.loadFromFile("images/click.png")) {
        cerr << "Fail load" << endl;
        return -1;
    }
    if (!infoTexture.loadFromFile("images/infoBox.png")) {
        cerr << "Fail load" << endl;
        return -1;
    }
    if (!btn1Textrue.loadFromFile("images/btn1.png")) {
        cerr << "Fail load" << endl;
        return -1;
    }
    if (!playerTexture.loadFromFile("images/playerImg.png")) {
        cerr << "Fail load" << endl;
        return -1;
    }

    // 배경
    Sprite startBackground(startBackgroundTexture);
    Sprite gameBackground(gameBackgroundTexture);
    Sprite badBackground(badendingTexture);
    Sprite happybackground(happyendingTexture);
    // 그외
    Sprite clickIcon(clickTexture);
    clickIcon.setPosition(
        (window.getSize().x - clickIcon.getTexture()->getSize().x) / 2.0f, // X 좌표
        (window.getSize().y - clickIcon.getTexture()->getSize().y) / 2.0f  // Y 좌표
    );
    Sprite infoBox(infoTexture);
    infoBox.setPosition( // 정가운데
        (window.getSize().x - infoBox.getTexture()->getSize().x) / 2.0f, // X 좌표
        (window.getSize().y - infoBox.getTexture()->getSize().y) / 2.0f  // Y 좌표
    );
    Sprite checkBtn(btn1Textrue);
    checkBtn.setPosition(
        (window.getSize().x - checkBtn.getTexture()->getSize().x) / 2.0f, 500.f
    );

    Sprite player(playerTexture);
    player.setPosition(
        (window.getSize().x - infoBox.getTexture()->getSize().x) / 2.0f + 100.f, 100.f
    );

    // 물고기 객체 생성 (30% 확률로 MiniFish 객체 생성)
    Fish* myFish;
    if (std::rand() % 100 < 30) {
        // 30% 확률로 MiniFish 객체 생성
        myFish = new MiniFish(230, "images/fish2_1.png", "images/fish2_2.png");
    }
    else {
        // 70% 확률로 Fish 객체 생성
        myFish = new Fish(250, "images/fish1_1.png", "images/fish1_2.png");
    }
    myFish->setPosition(0.0f, 500.0f);

    float fishSpeed = 0.05f;
    bool movingRight = true;

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    VertexArray line(Lines, 2);
    line[0].color = Color::Black;
    line[1].color = Color::Black;

    float lineSpeed = 0.5f;
    Vector2f targetPosition;
    bool lineMoving = false;
    bool collisionDetected = false;
    bool isFishing = true;
    bool fishCaught = false;
    bool showInfo = false;
    int clickCount = 0;
    int missCount = 0;
    int catchedFish = 0;
    int day = 1;
    int coinCount = 0;

    Font font1, font2, font3, font4;
    if (!font1.loadFromFile("fonts/런드리고딕OTF Bold.otf")) {
        return -1;
    }
    if (!font2.loadFromFile("fonts/전남교육유나체 TTF Regular.ttf")) {
        return -1;
    }
    if (!font3.loadFromFile("fonts/The Jamsil 3 Regular.ttf")) {
        return -1;
    }
    if (!font4.loadFromFile("fonts/The Jamsil 2 Light.ttf")) {
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

    // info 텍스트
    String text1 = L"획득 물고기"; // 한글로 나오게
    String text2 = myFish->getName();
    String text3 = L"크기 : " + to_string(myFish->getSize());
    String text4 = L"지역 : " + myFish->getArea();
    String text5 = L"희귀도 : " + myFish->getValue();
    String text6 = L"가격 : " + to_string(myFish->getCoin()) + L"coin";

    Text infoT_1(text1, font1, 27);
    infoT_1.setFillColor(Color::Black);
    infoT_1.setPosition(540.f, 210.f);
    Text infoT_2(text2, font3, 17);
    infoT_2.setFillColor(Color::Black);
    infoT_2.setPosition(550.f, 390.f);
    Text infoT_3(text3, font4, 15);
    infoT_3.setFillColor(Color::Black);
    infoT_3.setPosition(485.f, 420.f);
    Text infoT_4(text4, font4, 15);
    infoT_4.setFillColor(Color::Black);
    infoT_4.setPosition(620.f, 420.f);
    Text infoT_5(text5, font4, 15);
    infoT_5.setFillColor(Color::Black);
    infoT_5.setPosition(485.f, 450.f);
    Text infoT_6(text6, font4, 15);
    infoT_6.setFillColor(Color::Black);
    infoT_6.setPosition(620.f, 450.f);

    if (!fishInfoTexture.loadFromFile(myFish->getImg())) {
        cerr << "fail load";
        return -1;
    }
    Sprite fishInfoImg(fishInfoTexture);
    fishInfoImg.setPosition(
        (window.getSize().x - fishInfoImg.getTexture()->getSize().x) / 2.0f, 250.f
    );

    // status바
    RectangleShape status(Vector2f(1200, 60.f));
    status.setFillColor(Color::White);

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
                myFish->setDirection(movingRight);
                myFish->setPosition(movingRight ? 3.0f : 1170.0f, 500.0f);
            }
            else if (gameState == 1 && event.type == Event::MouseButtonPressed && !lineMoving && isFishing) {
                // player의 크기와 위치 계산
                FloatRect playerBounds = player.getGlobalBounds();
                Vector2f playerRightMiddle(
                    playerBounds.left + playerBounds.width,           // 오른쪽 끝 X 좌표
                    playerBounds.top + playerBounds.height / 2.0f - 40.f    // 세로 중앙 Y 좌표
                );

                // 선(line) 시작점 설정
                line[0].position = playerRightMiddle;
                line[1].position = playerRightMiddle;

                // 마우스 클릭 위치를 목표 위치로 설정
                targetPosition = Vector2f(event.mouseButton.x, event.mouseButton.y);

                // 상태 업데이트
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

            // infoBtn클릭 이벤트
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    // 클릭 위치가 checkBtn의 경계 안에 있는지 확인
                    if (checkBtn.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        showInfo = false; // 버튼 클릭 시 showInfo를 false로 설정
                    }
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
            window.draw(player);
            window.draw(line);
            window.draw(*myFish);  // myFish는 포인터이므로 dereference해서 그립니다
            window.draw(status);

            if (!collisionDetected) {
                myFish->update();
            }

            if (day == 4) {
                gameState = 2;
            }

            if (!lineMoving && !collisionDetected) {
                FloatRect fishBounds = myFish->getGlobalBounds();
                Vector2f lineEnd = line[1].position;
                Vector2f lineStart = line[0].position;

                // 물고기와 줄 끝이 충돌했는지 확인
                if (fishBounds.contains(lineEnd)) {
                    if(missCount > 0) missCount--;
                    cout << missCount << endl;
                    std::cout << "Collision detected: Fish touched the line!" << std::endl;
                    collisionDetected = true;
                    fishCaught = true;
                    isFishing = false;
                    coinCount += myFish->getCoin();
                    cout << coinCount << endl;
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

            // 충돌 발생 시 clickIcon 그리기
            if (collisionDetected) {
                if (clickCount < 10) {
                    window.draw(clickIcon);
                }
            }

            if (showInfo) {
                // 물고기를 잡을 때마다 불러오기
                if (!fishInfoTexture.loadFromFile(myFish->getImg())) {
                    cerr << "fail load";
                    return -1;
                }
                text2 = myFish->getName();
                text3 = L"크기 : " + to_string(myFish->getSize());
                text4 = L"지역 : " + myFish->getArea();
                text5 = L"희귀도 : " + myFish->getValue();
                text6 = L"가격 : " + to_string(myFish->getCoin()) + L"coin";

                window.draw(infoBox);
                window.draw(checkBtn);
                window.draw(infoT_1);
                window.draw(infoT_2);
                window.draw(infoT_3);
                window.draw(infoT_4);
                window.draw(infoT_5);
                window.draw(infoT_6);
                window.draw(fishInfoImg);
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
                            missCount++;
                            cout << "miss" << endl;
                            if (missCount == 3) gameState = 3;
                        }
                    }
                    else {
                        direction /= length;
                        line[1].position += direction * lineSpeed;
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

                    FloatRect fishBounds = myFish->getGlobalBounds();
                    Vector2f fishSize(fishBounds.width, fishBounds.height);
                    myFish->setPosition(line[1].position.x - fishSize.x / 2, line[1].position.y);
                    showInfo = true;
                }

                if (!showInfo) {
                    if (length <= lineSpeed) {
                        myFish->printInfo();
                        clickCount = 0;

                        movingRight = (rand() % 2 == 0);
                        if (rand() % 100 < 30) {
                            // 30% 확률로 MiniFish 객체 생성
                            delete myFish;
                            myFish = new MiniFish(230, "images/fish2_1.png", "images/fish2_2.png");
                        }
                        else {
                            // 70% 확률로 Fish 객체 생성
                            delete myFish;
                            myFish = new Fish(250, "images/fish1_1.png", "images/fish1_2.png");
                        }
                        myFish->setDirection(movingRight);
                        myFish->setPosition(movingRight ? 3.0f : 1170.0f, 500.0f);

                        lineMoving = false;
                        collisionDetected = false;
                        fishCaught = false;
                        isFishing = true;
                        catchedFish += 1;
                        if (catchedFish == 5) {
                            day += 1;
                            cout << "today : " << day << endl;
                        }
                    }
                }
            }
        }

        else if (gameState == 2) {
            window.draw(happybackground);
        }

        else if (gameState == 3) {
            window.draw(badBackground);
        }

        window.display();
    }

    delete myFish;  // 종료 전에 할당한 메모리 해제
    return 0;
}