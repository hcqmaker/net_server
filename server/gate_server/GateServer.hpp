#ifndef __GATESERVER_HPP__
#define __GATESERVER_HPP__

#include "net.hpp"
#include "base.hpp"
#include <boost/thread.hpp>

using namespace std;

NETWORK_BEGIN
class GateServer
{
public:
	GateServer();
	~GateServer();

	void run();
protected:
	class MasterHandle
	{
	public:
		MasterHandle(GateServer *s);
		~MasterHandle();

		void onReciveClientHandle(uint64 clientId, ByteBuffer& data);
		void onErrorClientHandle(IClient *client, const boost::system::error_code& error);
	protected:
		GateServer *m_pGateServer;
	};

	class LoginHandle
	{
	public:
		LoginHandle(GateServer *s);
		~LoginHandle();

		void onReciveClientHandle(uint64 clientId, ByteBuffer& data);
		void onErrorClientHandle(IClient *client, const boost::system::error_code& error);
	protected:
		GateServer *m_pGateServer;
	};

	class ChatHandle
	{
	public:
		ChatHandle(GateServer *s);
		~ChatHandle();

		void onReciveClientHandle(uint64 clientId, ByteBuffer& data);
		void onErrorClientHandle(IClient *client, const boost::system::error_code& error);
	protected:
		GateServer *m_pGateServer;
	};

	class ClientServerHandle
	{
	public:
		ClientServerHandle(GateServer *s);
		~ClientServerHandle();

		void onReciveServerHandle(uint64 serverId, uint64 sessionId, ByteBuffer& data);
		void onErrorServerHandle(IServer *server, const boost::system::error_code& error);
	protected:
		GateServer *m_pGateServer;
	};

	class GameServerHandle
	{
	public:
		GameServerHandle(GateServer *s);
		~GameServerHandle();

		void onReciveServerHandle(uint64 serverId, uint64 sessionId, ByteBuffer& data);
		void onErrorServerHandle(IServer *server, const boost::system::error_code& error);
	protected:
		GateServer *m_pGateServer;
	};

protected:
	IServer *m_pCServer; // client server
	IServer *m_pGServer; // game server

	IClient *m_pMaster;
	IClient *m_pLogin;
	IClient *m_pChat;
};

NETWORK_END

#endif // __GATESERVER_HPP__