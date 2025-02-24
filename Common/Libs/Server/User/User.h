#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <atomic>

class Hub;
class Session;
class ICommand;

class User : public std::enable_shared_from_this<User>
{
public:
	User(uint32_t id, boost::asio::io_context& io, std::shared_ptr<Hub> hub);
	~User() = default;
	
	std::shared_ptr<Session> CreateSession();
	void SessionStart();

	uint32_t GetID() const; //thread-safe
	void RequestAddEvent(std::shared_ptr<ICommand> command) const; //thread-safe
	void SendCommandToSession(const ICommand& command) const; //thread-safe

	void SetName(std::string_view name);
	std::string GetName();
	std::string_view GetName() const;

	std::shared_ptr<Session> GetSession(); //thread-safe

	void SetHub(std::shared_ptr<Hub> hubOrNull); //thread-local
	std::shared_ptr<Hub> GetHubOrNull() const; //thread-safe if Sethub is thread-local

	void CloseConnection(); //thread-safe
	bool IsConnectionClosed() const; //thread-safe

private:
	std::atomic<bool> mIsConnectionClosed = false;
	uint32_t mID;
	boost::asio::io_context& mIO;
	std::string mName; //thread-local
	std::weak_ptr<Hub> mHubOrNull; //Hub에서 다른 Hub로 이동중엔 nullptr을 가짐
	std::shared_ptr<Session> mSession;
};
