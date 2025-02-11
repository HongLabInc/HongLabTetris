#include "Button.h"
#include <iostream>
#include "../Managers/ConsoleColor.h"  // 콘솔 색상 관련 정의
#include "../Cell.h"                   // Cell 클래스

Button::Button()
    : InteractiveWidget(nullptr, nullptr)
{}

Button::Button(InputManager* inputManager, ConsoleFrame* frame)
    : InteractiveWidget(inputManager, frame)
{
    text = L"Button";
    width = 6;
    height = 3;
}

Button::~Button() {}

void Button::Update() {
    // 상태 업데이트 (필요 시 구현)
}

void Button::draw() {
    // 예시로 ConsoleFrame의 FillRectangle를 사용하여 버튼 배경을 그립니다.
    Cell buttonCell{};
    buttonCell.SetBackgroundColor(ConsoleColor::BrightCyan);
    mFrame->FillRectangle(posX - width / 2, posY - height / 2, width, height, buttonCell);
}

void Button::setText(const std::wstring& text) {
    this->text = text;
    mFrame->SetText(posX - width / 2, posY, text, static_cast<WORD>(ConsoleColor::BrightGreen));
}

std::wstring Button::getText() const {
    return text;
}

bool Button::contains(int mouseX, int mouseY) const {
    // posX, posY를 버튼의 중심 좌표라고 가정하고 사각형 범위로 검사합니다.
    Cell buttonCell{};
    buttonCell.SetBackgroundColor(ConsoleColor::BrightYellow);
    mFrame->FillRectangle(posX - width / 2, posY - height / 2, width, height, buttonCell);

    return (mouseX >= posX - width / 2 && mouseX < posX + width / 2 &&
            mouseY >= posY - height / 2 && mouseY < posY + height / 2);
}

void Button::setOnClick(std::function<void()> callback) {
    onClick = callback;
}

// InteractiveWidget 인터페이스 구현
void Button::OnMouseClick() {
    std::cout << "Do Func" << std::endl;
    if (onClick) {
        onClick();
    }
}
