#pragma once

#include <string>
#include <string_view>
#include <functional>

class IManager
{
public:
	enum class eClientState
	{
		Main,
		WaitResponseFromServer,
		WritingNickName,
		WaitNickNameResponse,
		Lobby,
	};

	IManager() = default;
	virtual ~IManager() = default;

	virtual void SetState(eClientState state) = 0;
	virtual eClientState GetState() const = 0;

	virtual void AddEventToMainLoop(std::function<void(void)> callback) = 0; //thread-safe
	virtual void PrintToOutputPanel(std::wstring_view message) = 0; //thread-safe
	virtual void SendCommandToServer(const ICommand& command) = 0; //thread-safe
	virtual void ScrollOutputPanel(std::size_t numLines) = 0;//thread-safe;
};
