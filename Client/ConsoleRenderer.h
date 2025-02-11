#pragma once
#include <Windows.h>
#include <vector>

class ConsoleFrame;

class ConsoleRenderer
{
public:
    ConsoleRenderer(int width, int height, float frameRate);
    ~ConsoleRenderer();

    void Clear();

    // 나중에 추가된 Frame이 위쪽에 위치합니다.
    // Frame끼리 겹치는 부분이 있는 경우 나중에 추가된 Frame이 우선권을 가집니다.
	ConsoleFrame* AddFrame(int positionX, int positionY, int width, int height);
    void RemoveFrame(ConsoleFrame* frame);

    ConsoleFrame* GetMainFrame();

    float GetFrameRate() const;
    float GetFrameTime() const;

    void SetBuffer(int row, int column, CHAR_INFO charInfo);
    void Render();

    //출력되는 셀에 좌우 공백 여부
    static constexpr bool LEFT_SPACE = false;
    static constexpr bool RIGHT_SPACE = true;
    static constexpr int CELL_WIDTH = 1 + (LEFT_SPACE ? 1 : 0) + (RIGHT_SPACE ? 1 : 0);


    POINT GetWindowPosition() const {
        return mWindowPosition;
    }
    RECT GetClientRect() const {
        return mClientRect;
    }
    SIZE GetFontSize() const {
        return mFontSize;
    }
    RECT GetWindowRect() const {
        return mWindowRect;
    }

private:
    void RenderFrame(ConsoleFrame* frame);

    int mWidth;
    int mHeight;
    int mActualWidth; //공백(LEFT, RIGHT SPACE)포함 실제 너비
    float mFrameRate;
    float mFrameTime;
    HANDLE mConsoleHandle;
    CHAR_INFO* mBuffer;

    ConsoleFrame* mMainFrame; // 바탕화면
    std::vector<ConsoleFrame*> mFrames;

    // 창 위치 및 크기 정보 추가
    POINT mWindowPosition;     // 창의 스크린 좌표
    RECT mClientRect;         // 클라이언트 영역
    SIZE mFontSize;          // 폰트 크기
    RECT mWindowRect;        // 전체 창 영역
};
