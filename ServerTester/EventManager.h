#pragma once

#include <memory>
#include <boost/asio.hpp>
#include "Hub/ClientHub.h"
#include "Command/Commands.h"
#include "Consoles.h"

class EventManager: public IManager
{
public :
	using IO = boost::asio::io_context;
	using eState = IManager::eClientState;

	EventManager(IO& main, IO& input, IO& network);
	virtual ~EventManager() final = default;

	void SetState(eState state) final;
	eState GetState() const final;

	void SetClientHub(std::shared_ptr<ClientHub> clientHub);
	void SetConsole(std::shared_ptr<Console> console);
	void SetOutputPanel(std::shared_ptr<Panel> outputPanel);

	void RunMainLoop(); //thread-safe
	void ProcessConsoleInput(std::wstring_view text); //thread-safe

	void pollInput(std::shared_ptr<Panel> inputPanel);

	virtual void AddEventToMainLoop(std::function<void(void)> callback) final;
	virtual void PrintToOutputPanel(std::wstring_view message) final;
	virtual void ScrollOutputPanel(std::size_t numLines) final;
	virtual void SendCommandToServer(const ICommand& command) final; //thread-safe

private:
	eState mState = eState::Main;
	std::shared_ptr<ClientHub> mClientHub;
	std::shared_ptr<Console> mConsole;
	std::shared_ptr<Panel> mOutputPanel;
	IO& mMainIO;
	IO& mInputIO;
	IO& mNetworkIO;
};
