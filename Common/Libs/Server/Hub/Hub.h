#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <boost/asio.hpp>
#include <string>
#include <string_view>

class Server;
class User;
class ICommand;
class Session;
class ILocalCommand;

class Hub: public std::enable_shared_from_this<Hub>
{
public:
	using Strand = boost::asio::strand<boost::asio::io_context::executor_type>;
	using Users = std::unordered_map<uint32_t, std::shared_ptr<User> >;

	explicit Hub(uint32_t id, boost::asio::io_context& io, std::size_t roomSize);
	virtual ~Hub() = default;

	void AddEvent(uint32_t requestUserID, std::shared_ptr<ICommand> command); //thread-safe

	void AddUser(std::shared_ptr<User> user);
	void RemoveUser(uint32_t userID);

	virtual void AddUserEvent(uint32_t userID);
	virtual void RemoveUserEvent(uint32_t userID);

	virtual void ShutDown();

	void BroadcastChatMessage(uint32_t userIdOrZero, std::string_view message) const;

	uint32_t GetID() const; //thread-safe
	Strand GetStrand() const; //thread-safe

	std::shared_ptr<User> GetUserOrNull(uint32_t userID);
	std::shared_ptr<Session> GetUserSessionOrNull(uint32_t userID);

	void SendCommandToUser(uint32_t userID, const ICommand& command) const; //thread-safe

	bool IsJoinable() const;
	bool HasUser(uint32_t userID) const;

	void RequestMoveUser(uint32_t userID, std::shared_ptr<Hub> targetHub);

	virtual void CloseConnection(); //Client Only

	virtual void RequestCompletionHandler(bool canMove, uint32_t userID);
	virtual void AcceptCompletionHandler(bool canAccept, uint32_t userID);

protected:
	bool mCanEnter = true; // 상속 클래스에서 필요한 경우 조작
	uint32_t mID;
	Users mUsers;
	boost::asio::io_context& mIO;
	Strand mStrand;

private:
	void replyRequest(std::shared_ptr<Hub>& targetHub, uint32_t userID,
					  Hub::Strand& targetStrand, std::shared_ptr<Hub>& self);
	void sendUser(uint32_t userID, std::shared_ptr<Hub>& targetHub, Strand& targetStrand,
				  std::shared_ptr<Hub>& self);
	void receiveUser(uint32_t userID, std::shared_ptr<Hub>& targetHub,
					 std::shared_ptr<User>& user);
};
