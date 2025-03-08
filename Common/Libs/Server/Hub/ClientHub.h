#pragma once

#include <functional>
#include <memory>
#include "Hub.h"
#include "IManager.h"

class ClientHub final: public Hub
{
public:
	explicit ClientHub(boost::asio::io_context& io, IManager& eventManager);
	~ClientHub() = default;

	void Connect(std::string_view ip, unsigned short port);
	virtual void CloseConnection() final;

	IManager& GetEventManager();

	void AddEventToMainLoop(std::function<void (void)> callback);

private:
	IManager& mEventManager;
	bool mConnected = false;
};
