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
    mActualWidth = mWidth * CELL_WIDTH;

    mConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = {(SHORT)mActualWidth, (SHORT)mHeight};
    SetConsoleScreenBufferSize(mConsoleHandle, bufferSize);

    CONSOLE_CURSOR_INFO cursorInfo = {1, FALSE};
    SetConsoleCursorInfo(mConsoleHandle, &cursorInfo);

    mBuffer = new CHAR_INFO[mActualWidth * mHeight];
    mMainFrame = new ConsoleFrame(0, 0, width, height);
}

ConsoleRenderer::~ConsoleRenderer()
{
    Clear();
    delete mMainFrame;
    delete[] mBuffer;
}

void ConsoleRenderer::Clear()
{
    for (auto frame : mFrames)
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
    if (frame == nullptr)
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