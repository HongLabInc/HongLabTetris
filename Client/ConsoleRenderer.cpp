#include <cassert>
#include "ConsoleRenderer.h"
#include "ConsoleFrame.h"
#include <iostream>

ConsoleRenderer::ConsoleRenderer(int width, int height, float frameRate)
    : mWidth(width)
    , mHeight(height)
    , mFrameRate(frameRate)
    , mFrameTime(1.0f / frameRate)
{
    // 콘솔 핸들 및 기본 설정
    mConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (mConsoleHandle == INVALID_HANDLE_VALUE) {
        // 에러 처리
        throw std::runtime_error("Failed to get console handle");
    }

    // 폰트 정보 설정
    CONSOLE_FONT_INFOEX cfi = {sizeof(cfi)};
    if (GetCurrentConsoleFontEx(mConsoleHandle, FALSE, &cfi)) {
        mFontSize.cx = cfi.dwFontSize.X;
        mFontSize.cy = cfi.dwFontSize.Y;
    }

    // 버퍼 크기 설정
    mActualWidth = mWidth * CELL_WIDTH;
    COORD bufferSize = {(SHORT)mActualWidth, (SHORT)mHeight};

    // 창 크기 먼저 설정
    SMALL_RECT windowSize = {0, 0, (SHORT)(mActualWidth - 1), (SHORT)(mHeight - 1)};
    SetConsoleWindowInfo(mConsoleHandle, TRUE, &windowSize);

    // 버퍼 크기 설정
    if (!SetConsoleScreenBufferSize(mConsoleHandle, bufferSize)) {
        throw std::runtime_error("Failed to set console buffer size");
    }

    // 커서 설정
    CONSOLE_CURSOR_INFO cursorInfo = {1, FALSE};
    SetConsoleCursorInfo(mConsoleHandle, &cursorInfo);

    // 창 위치 및 크기 정보 초기화
    HWND consoleWindow = GetConsoleWindow();
    if (consoleWindow) {
        GetWindowRect();
        GetClientRect();

        mWindowPosition.x = mWindowRect.left;
        mWindowPosition.y = mWindowRect.top;

        // 디버깅용 출력
        std::wcout << L"Window Position: (" << mWindowPosition.x << L", "
            << mWindowPosition.y << L")\n";
        std::wcout << L"Client Area: " << (mClientRect.right - mClientRect.left)
            << L"x" << (mClientRect.bottom - mClientRect.top) << L"\n";
        std::wcout << L"Font Size: " << mFontSize.cx << L"x" << mFontSize.cy << L"\n";
    }

    // 버퍼 할당
    mBuffer = new CHAR_INFO[mActualWidth * mHeight];
    mMainFrame = new ConsoleFrame(0, 0, width, height);

    // 초기 지연을 주어 창이 완전히 초기화되도록 함
    Sleep(100);

    // 초기화 후 최종 위치 다시 확인
    if (consoleWindow) {
        GetWindowRect();
        mWindowPosition.x = mWindowRect.left;
        mWindowPosition.y = mWindowRect.top;
    }

}

ConsoleRenderer::~ConsoleRenderer()
{
    Clear();
    delete mMainFrame;
    delete[] mBuffer;
}

void ConsoleRenderer::Clear()
{
    for(auto frame : mFrames)
    {
        delete frame;
    }
    mFrames.clear();

    assert(mMainFrame != nullptr);
    mMainFrame->Clear();
}

ConsoleFrame* ConsoleRenderer::AddFrame(int positionX, int positionY, int width, int height)
{
    ConsoleFrame* output = new ConsoleFrame(positionX, positionY, width, height);
    mFrames.push_back(output);
    return output;
}

ConsoleFrame * ConsoleRenderer::GetMainFrame()
{
    return mMainFrame;
}

float ConsoleRenderer::GetFrameRate() const
{
    return mFrameRate;
}

float ConsoleRenderer::GetFrameTime() const
{
    return mFrameTime;
}

void ConsoleRenderer::RemoveFrame(ConsoleFrame* frame)
{
    if(frame == nullptr)
        return;

    auto it = std::find(mFrames.begin(), mFrames.end(), frame);

    if (it != mFrames.end())
    {
        delete *it;
        mFrames.erase(it);
    }
}

void ConsoleRenderer::SetBuffer(int row, int column, CHAR_INFO charInfo)
{
    assert(0 <= row && row < mHeight);
    assert(0 <= column && column < mActualWidth);

    mBuffer[row * mActualWidth + column] = charInfo;
}

void ConsoleRenderer::RenderFrame(ConsoleFrame* frame)
{
    for (int row = 0; row < frame->GetHeight(); ++row)
    {
        for (int column = 0; column < frame->GetWidth(); ++column)
        {
            int globalRow = frame->GetY() + row;
            int globalColumn = (frame->GetX() + column) * CELL_WIDTH;

            const Cell& cell = frame->GetCell(column, row);

            if (LEFT_SPACE)
            {
                SetBuffer(globalRow, globalColumn, {L' ', cell.GetAttributes()});
                ++globalColumn;
            }

            SetBuffer(globalRow, globalColumn, cell.ToCharInfo());
            ++globalColumn;

            if (RIGHT_SPACE)
            {
                SetBuffer(globalRow, globalColumn, {L' ', cell.GetAttributes()});
            }
        }
    }
}

void ConsoleRenderer::Render()
{
    assert(mMainFrame != nullptr);
	RenderFrame(mMainFrame);

    for (const auto& frame : mFrames)
    {
        RenderFrame(frame);
    }

    SMALL_RECT writeRegion = {0, 0, (SHORT)(mActualWidth - 1), (SHORT)(mHeight - 1)};
    COORD bufferSize = {(SHORT)mActualWidth, (SHORT)mHeight};
    COORD bufferCoord = {0, 0};

    WriteConsoleOutputW(mConsoleHandle, mBuffer, bufferSize, bufferCoord, &writeRegion);
}
