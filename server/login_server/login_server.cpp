
#include <iostream>

#include "net.hpp"
#include "base.hpp"

#include "rule.hpp"
#include "login_handle.hpp"

#include <boost/thread.hpp>

using namespace std;


NETWORK_BEGIN

ClientHandle g_clientHandle[RULE_NUM] = {0};
SessionHandle g_sessionHandle[RULE_NUM] = {0};

class LoginServer
{
public:
	LoginServer()
		: m_pDataBase(0)
		, m_pServer(0)
		, m_pMaster(0)
	{

	}

	~LoginServer()
	{

	}

	void run()
	{

		Config cfg;
		cfg.loadIni("option.ini");
		
		std::string master_host = cfg.getString("master_host");
		int master_port = cfg.getInt("master_port");

		std::string db_host = cfg.getString("db_host");
		int db_port = cfg.getInt("db_port");

		int listener_port = cfg.getInt("listener_port");

		m_pDataBase = NetLib::instance().createClient(db_host.c_str(), db_port);
		m_pMaster = NetLib::instance().createClient(master_host.c_str(), master_port);
		m_pServer = NetLib::instance().createServer(listener_port);

		m_pServer->bindReceiveHandle(boost::bind(&LoginServer::onReciveServerHandle, this, _1, _2, _3));
		m_pServer->bindErrorHandle(boost::bind(&LoginServer::onErrorServerHandle, this, _1, _2));

		m_pDataBase->bindReceiveHandle(boost::bind(&LoginServer::onReciveDBHandle, this, _1, _2));
		m_pDataBase->bindErrorHandle(boost::bind(&LoginServer::onErrorDBHandle, this, _1, _2));

		m_pMaster->bindReceiveHandle(boost::bind(&LoginServer::onReciveMasterHandle, this, _1, _2));
		m_pMaster->bindErrorHandle(boost::bind(&LoginServer::onErrorMasterHandle, this, _1, _2));

		register_session(g_sessionHandle);
		register_client(g_clientHandle);
		NetLib::instance().run();
	}
	// server
	void onReciveServerHandle(uint64 serverId, uint64 sessionId, ByteBuffer& data)
	{
		uint16 cmd = data.read<uint16>();
		SessionHandle handle = g_sessionHandle[cmd];
		if (handle)
			handle(m_pDataBase, serverId, sessionId, data);
		else
			sLog.outWarning("[onReciveServerHandle] can't find handle cmd:%d", cmd);
	}

	void onErrorServerHandle(IServer *server, const boost::system::error_code& error)
	{
		uint64 serverId = server->getId();
		uint16 port = server->getPort();

		NetLib::instance().destroyServer(serverId);
		sLog.outError("server error serverId:%lld port: %d error:%s \n", serverId, port, error.message().c_str());
	}

	// client
	void onReciveDBHandle(uint64 clientId, ByteBuffer& data)
	{
		uint16 cmd = data.read<uint16>();
		ClientHandle handle = g_clientHandle[cmd];
		if (handle)
			handle(m_pServer, clientId, data);
		else
			sLog.outWarning("[onReciveDBHandle] can't find handle cmd:%d", cmd);
	}

	void onErrorDBHandle(IClient *client, const boost::system::error_code& error)
	{
		uint64 clientId = client->getId();
		std::string host = client->getHost();
		int port = client->getPort();

		NetLib::instance().destroyClient(clientId);
		sLog.outError("client error clientId:%lld host:%s port:%d error:%s \n", clientId, host.c_str(), port, error.message().c_str());

	}
	// client
	void onReciveMasterHandle(uint64 clientId, ByteBuffer& data)
	{
		uint16 cmd = data.read<uint16>();
		sLog.outMessage("[onReciveMasterHandle] cmd: %d", cmd);
	}

	void onErrorMasterHandle(IClient *client, const boost::system::error_code& error)
	{
		uint64 clientId = client->getId();
		std::string host = client->getHost();
		uint16 port = client->getPort();

		NetLib::instance().destroyClient(clientId);
		sLog.outError("client error clientId:%lld host:%s port:%d error:%s \n", clientId, host.c_str(), port, error.message().c_str());
	}
protected:
	IServer *m_pServer;
	IClient *m_pDataBase;
	IClient *m_pMaster;
};


NETWORK_END


int main(int argc, char ** argv)
{
	USE_NETWORK;
	LoginServer app;
	app.run();

	return 0;
};