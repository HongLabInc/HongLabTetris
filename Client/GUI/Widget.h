#ifndef WIDGET_H
#define WIDGET_H

#include <string>
#include "../Managers/InputManager.h"
#include "../ConsoleFrame.h"
#include "../ConsoleRenderer.h"

// Widget은 GUI 컴포넌트의 기본 클래스
class Widget {
public:
    Widget() = default;
    Widget(InputManager* im, ConsoleFrame* frame)
        : mInputManager(im), mFrame(frame) {}
    virtual ~Widget() = default;
    virtual void Update() = 0;
    virtual void draw() = 0;

    // 기본 resize 함수: 위젯의 크기를 변경
    virtual void resize(int width, int height) {
        this->width = width;
        this->height = height;
    }

    virtual void SetPosition(int x, int y) {
        posX = x;
        posY = y;
    }

    // 위젯의 이름 설정
    void setName(const std::string& name) {
        this->name = name;
    }
    std::string getName() const {
        return name;
    }

    // 기본적으로 입력 이벤트 대상이 아니라면 false를 반환
    virtual bool contains(int mouseX, int mouseY) const {
        return false;
    }

protected:
    std::string name;   // 위젯 이름
    int width = 0;      // 위젯 가로 크기
    int height = 0;     // 위젯 세로 크기
    int posX = 0;       // 위젯의 좌측 상단(혹은 중앙) X 좌표
    int posY = 0;       // 위젯의 좌측 상단(혹은 중앙) Y 좌표

    InputManager* mInputManager = nullptr;
    ConsoleFrame* mFrame = nullptr;
};

#endif // WIDGET_H