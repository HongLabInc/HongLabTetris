#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <vector>
#include <algorithm>
#include <windows.h>
#include "../GUI/InteractiveWidget.h"

class UIManager {
public:
    UIManager() = default;
    ~UIManager();

    // 위젯 추가
    void AddWidget(Widget* widget);

    // 위젯 제거
    void RemoveWidget(Widget* widget);

    // 마우스 이벤트 처리: 입력 처리가 가능한 위젯(예: Button)만 대상으로 함
    void ProcessMouseEvent(const MOUSE_EVENT_RECORD& mouseEvent);

    // 모든 위젯 업데이트
    void Update();

    // 모든 위젯 그리기
    void draw();

private:
    std::vector<Widget*> mWidgets;
};

#endif // UIMANAGER_H
