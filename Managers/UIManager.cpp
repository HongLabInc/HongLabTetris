#include "UIManager.h"


UIManager::~UIManager() {
    for (Widget* widget : mWidgets) {
        delete widget;
    }
    mWidgets.clear();
}

// 위젯 추가
void UIManager::AddWidget(Widget * widget) {
    if (widget)
        mWidgets.push_back(widget);
}

// 위젯 제거
void UIManager::RemoveWidget(Widget * widget) {
    auto it = std::find(mWidgets.begin(), mWidgets.end(), widget);
    if (it != mWidgets.end()) {
        delete *it;
        mWidgets.erase(it);
    }
}

// 마우스 이벤트 처리: 입력 처리가 가능한 위젯(예: Button)만 대상으로 함

void UIManager::ProcessMouseEvent(const MOUSE_EVENT_RECORD & mouseEvent) {
    int mouseX = mouseEvent.dwMousePosition.X;
    int mouseY = mouseEvent.dwMousePosition.Y;

    for (Widget* widget : mWidgets) {
        if (auto interactive = dynamic_cast<InteractiveWidget*>(widget)) {
            if (interactive->contains(mouseX, mouseY)) {
                interactive->OnMouseClick();
                break;  // 하나의 위젯만 처리하도록 한다면 break
            }
        }
    }
}

// 모든 위젯 업데이트
void UIManager::Update() {
    for (Widget* widget : mWidgets) {
        widget->Update();
    }
}

// 모든 위젯 그리기
void UIManager::draw() {
    for (Widget* widget : mWidgets) {
        widget->draw();
    }
}
