// Button.h
#ifndef BUTTON_H
#define BUTTON_H

#include "InteractiveWidget.h"
#include <functional>
#include <string>

class Button: public InteractiveWidget {
public:
    Button();
    Button(InputManager* inputManager, ConsoleFrame* frame);
    virtual ~Button();

    // Widget 인터페이스 구현
    virtual void Update() override;
    virtual void draw() override;
    virtual bool contains(int mouseX, int mouseY) const override;

    // 텍스트 관련 메서드
    void setText(const std::wstring& newText);
    std::wstring getText() const;

    // 이벤트 콜백 설정
    void SetOnHoverEnter(std::function<void()> callback);
    void SetOnHoverExit(std::function<void()> callback);
    void SetOnMouseDown(std::function<void()> callback);
    void SetOnMouseUp(std::function<void()> callback);
    void SetOnClick(std::function<void()> callback);

private:
    void HandleMouseEvent(const MouseEvent& event);

    std::wstring text;
    size_t mListenerIndex;  // 이벤트 리스너 인덱스 저장

    // 이벤트 콜백
    std::function<void()> onHoverEnter;
    std::function<void()> onHoverExit;
    std::function<void()> onMouseDown;
    std::function<void()> onMouseUp;
    std::function<void()> onClick;

    // 상태 플래그
    bool mIsHovered;
    bool mIsPressed;

};

#endif // BUTTON_H