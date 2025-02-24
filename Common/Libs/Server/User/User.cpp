#include <cassert>
#include <boost/asio.hpp>
#include "User.h"
#include "Hub/Hub.h"
#include "Session.h"

User::User(uint32_t id, boost::asio::io_context& io, std::shared_ptr<Hub> hub)
	: mID(id)
	, mIO(io)
	, mHubOrNull(std::move(hub))
{
}

std::shared_ptr<Session> User::CreateSession()
{
	mSession = std::make_shared<Session>(shared_from_this(), mIO);
	return mSession;
}

void User::SessionStart()
{
	mSession->Start();
}

uint32_t User::GetID() const
{
    return mID;
}

void User::RequestAddEvent(std::shared_ptr<ICommand> command) const
{
	std::shared_ptr<Hub> hub = GetHubOrNull();
	if (hub != nullptr)
	{
		hub->AddEvent(mID, std::move(command));
	}
}

void User::SendCommandToSession(const ICommand& command) const
{
	if (mSession != nullptr)
	{
		mSession->Send(command);
	}
}

void User::SetName(std::string_view name)
{
    mName = name;
}

std::string User::GetName()
{
    return mName;
}

std::string_view User::GetName() const
{
    return mName;
}

std::shared_ptr<Session> User::GetSession()
{
	return mSession;
}

void User::SetHub(std::shared_ptr<Hub> hubOrNull)
{
	//SetHub은 Hub에서만 호출하므로 thread-local로 이루어집니다.
    mHubOrNull = hubOrNull;
}

std::shared_ptr<Hub> User::GetHubOrNull() const
{
	//SetHub가 thread-local인 경우 thread-safe입니다.
    return mHubOrNull.lock();
}

void User::CloseConnection()
{
	mIsConnectionClosed = true;
	if (std::shared_ptr<Hub> hub = mHubOrNull.lock())
	{
		boost::asio::dispatch
		(hub->GetStrand(),
		[this, hub]()
		{
			if (hub->HasUser(mID) == false)
			{
				return;
			}
			hub->RemoveUser(mID);
		});
	}
}

bool User::IsConnectionClosed() const
{
	return mIsConnectionClosed;
}
