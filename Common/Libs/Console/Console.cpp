#define NOMINMAX
#include <cassert>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <mutex>
#include "Console.h"
#include "Panel/Panel.h"

Console::Console(std::size_t height, std::size_t width)
	: mHeight(height)
	, mWidth(width)
{
	mBuffer.resize(mHeight * mWidth, console::DefaultCell);
	mDirtyCells.resize(mHeight * mWidth, false);
}

void Console::Init()
{
#ifdef _WIN32
	// 콘솔 API는 최신 Windows 에서 대부분 deprecated 되어 터미널별로 일부 동작이 다를 수 있습니다.
	SMALL_RECT windowRect
	{
		.Left = 0,
		.Top = 0,
		.Right = static_cast<SHORT>(2 * mWidth - 1), //멀티바이트 출력을 위해 2배로 설정
		.Bottom = static_cast<SHORT>(mHeight - 1)
	};

	if (!SetConsoleWindowInfo(mConsoleOutput, TRUE, &windowRect))
	{
		throw std::runtime_error("SetConsoleWindowInfo failed");
	}
#endif
}

std::shared_ptr<Panel> Console::AddPanel(std::size_t top, std::size_t left,
										 std::size_t height, std::size_t width)
{
	auto self = shared_from_this();
	auto panel = std::make_shared<Panel>(top, left, height, width, std::move(self));
	mPanels.push_back(panel);
	return panel;
}

void Console::RemovePanel(const std::shared_ptr<Panel>& panel)
{
	assert(panel != nullptr && "panel is nullptr");
	auto it = std::find(mPanels.begin(), mPanels.end(), panel);
	mPanels.erase(it);
}

void Console::WriteRegion(Rect rect, const std::vector<Cell>& buffer,
						  std::vector<uint8_t>& dirtyFlags)
{
	assert(rect.width * rect.height == buffer.size() && "buffer size mismatch");
	assert(rect.width * rect.height == dirtyFlags.size() && "dirtyFlags size mismatch");

	const std::size_t numCopyRows = std::min(rect.height, mHeight - rect.top);
	const std::size_t numCopyColumns = std::min(rect.width, mWidth - rect.left);

	for (std::size_t row = 0; row < numCopyRows; ++row)
	{
		auto bufferSrc = buffer.begin() + row * rect.width;
		auto bufferDest = mBuffer.begin() + (rect.top + row) * mWidth + rect.left;
		std::copy(bufferSrc, bufferSrc + numCopyColumns, bufferDest);

		auto dirtyFlagsSrc = dirtyFlags.begin() + row * rect.width;
		auto dirtyFlagsDest = mDirtyCells.begin() + (rect.top + row) * mWidth + rect.left;
		std::transform(dirtyFlagsSrc,
					   dirtyFlagsSrc + numCopyColumns,
					   dirtyFlagsDest,
					   dirtyFlagsDest,
					   std::logical_or<uint8_t>());
	}
	std::fill(dirtyFlags.begin(), dirtyFlags.end(), false);
}

void Console::Refresh()
{
	for (auto& panel : mPanels)
	{
		WriteRegion(panel->GetInfo(), panel->GetBuffer(), panel->GetDirtyFlags());
	}
	PrintDirtyBuffer();
	std::fill(mBuffer.begin(), mBuffer.end(), console::DefaultCell);
	std::fill(mDirtyCells.begin(), mDirtyCells.end(), false);
}

void Console::Clear()
{
	mPanels.clear();
	std::fill(mBuffer.begin(), mBuffer.end(), console::DefaultCell);
	std::fill(mDirtyCells.begin(), mDirtyCells.end(), true);
	ClearScreen();
}

void Console::RefreshWithDirtyMap()
{
	for (auto& panel : mPanels)
	{
		WriteRegion(panel->GetInfo(), panel->GetBuffer(), panel->GetDirtyFlags());
	}
	PrintDebugDirtyMap();
	PrintDirtyBuffer();
	std::fill(mBuffer.begin(), mBuffer.end(), console::DefaultCell);
	std::fill(mDirtyCells.begin(), mDirtyCells.end(), false);
}

std::size_t Console::GetHeight() const
{
	return mHeight;
}

std::size_t Console::GetWidth() const
{
	return mWidth;
}
