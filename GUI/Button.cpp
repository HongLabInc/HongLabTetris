#include "Button.h"
#include <iostream>

Button::Button() {
    text = L"Button";
    width = 6;
    height = 3;
}

Button::Button(InputManager * inputManager, ConsoleFrame* frame)
    : Widget(inputManager, frame)
{
    text = L"Button";
    width = 6;
    height = 3;
}

Button::~Button() {
}

void Button::Update() {
    
}

void Button::draw() {
    Cell buttonCell{};
    buttonCell.SetBackgroundColor(ConsoleColor::BrightRed);
    mFrame->FillRectangle(posX - width / 2, posY - height / 2, width, height, buttonCell);
}

void Button::setText(const std::wstring& text) {
    mFrame->SetText(posX - width / 2, posY, text, static_cast<WORD>(ConsoleColor::BrightGreen));
}

std::wstring Button::getText() const {
    return text;
}

bool Button::contains(int mouseX, int mouseY) const {
    return (mouseX >= posX - width / 2 && mouseX < posX + width / 2 &&
        mouseY >= posY - height / 2 && mouseY < posY + height / 2);
}

void Button::setOnClick(std::function<void()> callback) {
    onClick = callback;
}

void Button::click() {
    // 등록된 콜백 함수가 있다면 호출합니다.
    std::cout << "Do Func" << std::endl;
    if (onClick) {
        onClick();
    }
}