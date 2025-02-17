#define NOMINMAX
#include <cassert>
#include <algorithm>
#include <functional>
#include "Console.h"
#include "Panel/Panel.h"

Console::Console(std::size_t height, std::size_t width)
	: mHeight(height)
	, mWidth(width)
{
	mBuffer.resize(mHeight * mWidth, console::DefaultCell);
	mDirtyCells.resize(mHeight * mWidth, false);
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
	assert(panel != nullptr);
	auto it = std::find(mPanels.begin(), mPanels.end(), panel);
	mPanels.erase(it);
}

void Console::WriteRegion(Rect rect, const std::vector<Cell>& buffer,
						  std::vector<uint8_t>& dirtyFlags)
{
	assert(rect.width * rect.height == buffer.size());
	assert(rect.width * rect.height == dirtyFlags.size());

	const std::size_t numCopyRows = std::min(rect.height, mHeight - rect.top);
	const std::size_t numCopyColumns = std::min(rect.width, mWidth - rect.left);

	for (std::size_t row = 0; row < numCopyRows; ++row)
	{
		{
			auto srcStart = buffer.begin() + row * rect.width;
			auto destStart = mBuffer.begin() + (rect.top + row) * mWidth + rect.left;
			std::copy(srcStart, srcStart + numCopyColumns, destStart);
		}

		{
			auto srcStart = dirtyFlags.begin() + row * rect.width;
			auto destStart = mDirtyCells.begin() + (rect.top + row) * mWidth + rect.left;
			std::transform(srcStart, srcStart + numCopyColumns, destStart, destStart,
						   std::logical_or<uint8_t>());
		}
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
	std::fill(mDirtyCells.begin(), mDirtyCells.end(), false);
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
