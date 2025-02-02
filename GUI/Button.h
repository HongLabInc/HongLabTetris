#ifndef BUTTON_H
#define BUTTON_H

#include "Widget.h"
#include <string>
#include <functional>

// Button은 Widget을 상속받아 구현한 GUI 컴포넌트입니다.
class Button: public Widget {
public:
    Button();
    Button(InputManager* inputManager, ConsoleFrame* frame);
    virtual ~Button();

    virtual void Update() override;
    virtual void draw() override;

    void setText(const std::wstring& text);
    std::wstring getText() const;

    bool contains(int mouseX, int mouseY) const;

    // 버튼 클릭 시 실행될 콜백 함수를 설정하는 함수
    void setOnClick(std::function<void()> callback);

    // 버튼 클릭 이벤트를 외부에서 직접 트리거할 수 있도록 하는 함수
    void click();

private:
    std::wstring text;                     // 버튼에 표시할 텍스트
    std::function<void()> onClick;        // 클릭 이벤트 콜백 함수
};

#endif // BUTTON_H
