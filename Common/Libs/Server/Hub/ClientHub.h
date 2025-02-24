#pragma once

#include <memory>
#include "Hub.h"

class ILocalCommand;

class ClientHub final: public Hub
{
public:
	explicit ClientHub(boost::asio::io_context& io, ILocalCommandExecutor& localExecutor);
	~ClientHub() = default;

	void Connect(std::string_view ip, unsigned short port);

	virtual void CloseConnection() final;

private:
};
