// Button.cpp
#include "Button.h"
#include "../Managers/ConsoleColor.h"
#include "../Cell.h"

Button::Button()
    : InteractiveWidget(nullptr, nullptr)
    , text(L"Button")
    , mIsHovered(false)
    , mIsPressed(false)
    , mIsSelected(false)
{}

Button::Button(InputManager* inputManager, ConsoleFrame* frame)
    : InteractiveWidget(inputManager, frame)
    , text(L"Button")
    , mIsHovered(false)
    , mIsPressed(false)
    , mIsSelected(false)
    , mListenerIndex(0)
{
    if (inputManager) {
        mListenerIndex = inputManager->AddMouseListener(
            [this](const MouseEvent& event) {
            HandleMouseEvent(event);
        }
        );
    }
}

Button::~Button() {
    if (mInputManager) {
        mInputManager->RemoveMouseListener(mListenerIndex);
    }
}

void Button::Update() {
    if (!mInputManager || !mFrame) return;
}

void Button::draw() {
    if (!mFrame) return;

    Cell buttonCell{};

    buttonCell.SetBackgroundColor(
        mIsSelected ? ConsoleColor::BrightRed :
        mIsPressed ? ConsoleColor::BrightBlue :
        mIsHovered ? ConsoleColor::BrightCyan :
                    ConsoleColor::BrightGreen
    );

    mFrame->FillRectangle(
        posX - width,           // x 좌표를 posX로 변경
        posY - height / 2 + (height % 2 == 0 ? 0 : 1),          // y 좌표를 posY로 변경
        width,
        height,
        buttonCell
    );


    // 텍스트 위치 조정
    int textX = posX + (width - static_cast<int>(text.length())) / 2 - width;
    int textY = posY + height / 2;
    mFrame->SetText(
        textX,
        textY,
        text,
        static_cast<WORD>(
        mIsSelected ? ConsoleColor::White :
        mIsPressed ? ConsoleColor::White :
        mIsHovered ? ConsoleColor::BrightYellow :
        ConsoleColor::White
    )
    );


}

bool Button::contains(int mouseX, int mouseY) const {
    // 절대 좌표 기준으로 계산
    return (mouseX >= posX - width &&
            mouseX <=  posX + width &&
            mouseY >= posY - height / 2 &&
            mouseY <= posY + height / 2);
}

void Button::setText(const std::wstring& newText) {
    text = newText;
}

std::wstring Button::getText() const {
    return text;
}

void Button::SetOnHoverEnter(std::function<void()> callback) {
    onHoverEnter = callback;
}

void Button::SetOnHoverExit(std::function<void()> callback) {
    onHoverExit = callback;
}

void Button::SetOnMouseDown(std::function<void()> callback) {
    onMouseDown = callback;
}

void Button::SetOnMouseUp(std::function<void()> callback) {
    onMouseUp = callback;
}

void Button::SetOnClick(std::function<void()> callback) {
    onClick = callback;
}

void Button::SetSelected(bool selected) {
    mIsSelected = selected;
}

bool Button::IsSelected() const {
    return mIsSelected;
}

void Button::HandleMouseEvent(const MouseEvent& event) {
    if (!mFrame) return;

    POINT framePos = {
        event.position.x - mFrame->GetX(),
        event.position.y - mFrame->GetY()
    };

    bool isInside = contains(framePos.x, framePos.y);


    // 호버 상태 업데이트
    if (isInside != mIsHovered) {
        mIsHovered = isInside;
        if (mIsHovered && onHoverEnter) {
            onHoverEnter();
        } else if (!mIsHovered && onHoverExit) {
            onHoverExit();
        }
    }

    // 마우스 버튼 이벤트 처리
    if (event.button == MOUSE_LEFT) {
        if (isInside) {
            if (event.isPressed) {
                mIsPressed = true;
                if (onMouseDown) onMouseDown();
            } 
            else if (mIsPressed) {
                mIsPressed = false;
                if (onMouseUp) onMouseUp();
                if (onClick) onClick();
            }
        } else if (!event.isPressed) {
            mIsPressed = false;
        }
    }
}