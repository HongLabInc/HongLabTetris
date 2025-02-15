// Button.cpp
#include "Button.h"
#include "../Managers/ConsoleColor.h"
#include "../Cell.h"

Button::Button()
    : InteractiveWidget(nullptr, nullptr)
    , text(L"Button")
    , mIsHovered(false)
    , mIsPressed(false)
{}

Button::Button(InputManager* inputManager, ConsoleFrame* frame)
    : InteractiveWidget(inputManager, frame)
    , text(L"Button")
    , mIsHovered(false)
    , mIsPressed(false)
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
        mIsPressed ? ConsoleColor::White :
        mIsHovered ? ConsoleColor::BrightYellow :
        ConsoleColor::BrightGreen
    )
    );

    if (mInputManager) {
        POINT mousePos = mInputManager->GetMousePosition();
        POINT framePos = {
            mousePos.x - mFrame->GetX(),
            mousePos.y - mFrame->GetY()
        };
        mFrame->SetText(0, 1,
            L"Mouse: screen(" + std::to_wstring(mousePos.x) + L"," + std::to_wstring(mousePos.y) +
            L") frame(" + std::to_wstring(framePos.x) + L"," + std::to_wstring(framePos.y) + L")",
            static_cast<WORD>(ConsoleColor::White));

        mFrame->SetText(0, 2,
            L"Button: pos(" + std::to_wstring(posX) + L"," + std::to_wstring(posY) +
            L") size(" + std::to_wstring(width) + L"," + std::to_wstring(height) +
            L") hover:" + (mIsHovered ? L"Y" : L"N"),
            static_cast<WORD>(ConsoleColor::White));

        mFrame->SetText(0, 3,
             L"Button: pos(" + std::to_wstring(posX) + L"," + std::to_wstring(posY) +
            L") size(" + std::to_wstring(width) + L"," + std::to_wstring(height) +
            L") pressed:" + (mIsPressed ? L"Y" : L"N"),
            static_cast<WORD>(ConsoleColor::White));



    }
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

void Button::HandleMouseEvent(const MouseEvent& event) {
    if (!mFrame) return;

    POINT framePos = {
        event.position.x - mFrame->GetX(),
        event.position.y - mFrame->GetY()
    };

    mFrame->SetText(0, 3,
       L"HandleEvent: pos(" + std::to_wstring(framePos.x) + L"," + std::to_wstring(framePos.y) +
       L") button:" + std::to_wstring(static_cast<int>(event.button)) +
       L" pressed:" + (event.isPressed ? L"Y" : L"N"),
       static_cast<WORD>(ConsoleColor::White));

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