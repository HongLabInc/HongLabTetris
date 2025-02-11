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

    // InteractiveWidget 인터페이스 구현: 마우스 클릭 처리
    virtual void OnMouseClick() override;

    // 텍스트 관련 메서드
    void setText(const std::wstring& text);
    std::wstring getText() const;

    // 클릭 이벤트에 사용할 콜백 등록
    void setOnClick(std::function<void()> callback);

private:
    std::wstring text;
    std::function<void()> onClick;
};

#endif // BUTTON_H