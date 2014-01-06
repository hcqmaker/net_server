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
	class MasterHandle : public IClientHandle
	{
	public:
		MasterHandle(GateServer *s);
		~MasterHandle();

		virtual void onReciveClientHandle(uint64 clientId, ByteBuffer& data);
		virtual void onErrorClientHandle(IClient *client, const boost::system::error_code& error);
	protected:
		GateServer *m_pGateServer;
	};

	class LoginHandle : public IClientHandle
	{
	public:
		LoginHandle(GateServer *s);
		~LoginHandle();

		virtual void onReciveClientHandle(uint64 clientId, ByteBuffer& data);
		virtual void onErrorClientHandle(IClient *client, const boost::system::error_code& error);
	protected:
		GateServer *m_pGateServer;
	};

	class ChatHandle : public IClientHandle
	{
	public:
		ChatHandle(GateServer *s);
		~ChatHandle();

		virtual void onReciveClientHandle(uint64 clientId, ByteBuffer& data);
		virtual void onErrorClientHandle(IClient *client, const boost::system::error_code& error);
	protected:
		GateServer *m_pGateServer;
	};

	class ClientServerHandle : public IServerHandle
	{
	public:
		ClientServerHandle(GateServer *s);
		~ClientServerHandle();

		virtual void onReciveServerHandle(uint64 serverId, uint64 sessionId, ByteBuffer& data);
		virtual void onErrorServerHandle(IServer *server, const boost::system::error_code& error);
	protected:
		GateServer *m_pGateServer;
	};

	class GameServerHandle : public IServerHandle
	{
	public:
		GameServerHandle(GateServer *s);
		~GameServerHandle();

		virtual void onReciveServerHandle(uint64 serverId, uint64 sessionId, ByteBuffer& data);
		virtual void onErrorServerHandle(IServer *server, const boost::system::error_code& error);
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