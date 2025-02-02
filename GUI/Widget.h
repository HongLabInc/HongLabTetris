#ifndef WIDGET_H
#define WIDGET_H

#include <string>
#include <memory>
#include "../Managers/InputManager.h"
#include "../ConsoleFrame.h"

// Widget은 GUI 컴포넌트의 기본 클래스입니다.
class Widget {
public:
    Widget();
    Widget(InputManager* im, ConsoleFrame* frame);
    virtual ~Widget();

    virtual void Update() = 0;
    virtual void draw() = 0;

    // 기본 resize 함수: 위젯의 크기를 변경합니다.
    virtual void resize(int width, int height);

    virtual void SetPosition(int x, int y);

    // 위젯의 이름을 설정하고 가져오는 함수
    void setName(const std::string& name);
    std::string getName() const;

protected:
    std::string name; // 위젯의 이름
    int width;        // 위젯의 가로 크기
    int height;       // 위젯의 세로 크기

    int posX = 0;
    int posY = 0;

    InputManager* mInputManager = nullptr;
    ConsoleFrame* mFrame = nullptr;
};

#endif // WIDGET_H
