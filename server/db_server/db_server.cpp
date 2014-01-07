#include <iostream>

#include "net.hpp"
#include "base.hpp"
#include "rule.hpp"
#include "db.hpp"

#include "PlayerModule.hpp"
#include "function_handle.hpp"

#include <boost/thread.hpp>
#include <boost/function.hpp>

using namespace std;




NETWORK_BEGIN
	
	FuncHandle g_handles[RULE_NUM] = {0};
	Connection *g_conn = NULL;

	
class DBServer
{
public:
	DBServer()
		: m_pMaster(0)
		, m_pServer(0)
	{ }

	~DBServer() { }

	void run()
	{

		Config cfg;
		cfg.loadIni("option.ini");
		
		std::string master_host = cfg.getString("master_host");
		int master_port = cfg.getInt("master_port");

		std::string db_host = cfg.getString("db_host");
		std::string db_user = cfg.getString("db_user");
		std::string db_pwd = cfg.getString("db_pwd");

		int listener_port = cfg.getInt("listener_port");

		m_pMaster = NetLib::instance().createClient(master_host.c_str(), master_port);
		m_pServer = NetLib::instance().createServer(listener_port);

		m_pServer->bindReceiveHandle(boost::bind(&DBServer::onReciveServerHandle, this, _1, _2, _3));
		m_pServer->bindErrorHandle(boost::bind(&DBServer::onErrorServerHandle, this, _1, _2));

		m_pMaster->bindReceiveHandle(boost::bind(&DBServer::onReciveMasterHandle, this, _1, _2));
		m_pMaster->bindErrorHandle(boost::bind(&DBServer::onErrorMasterHandle, this, _1, _2));

		
		register_all(g_handles);

		g_conn = DbLib::instance().connect(db_host.c_str(), db_user.c_str(), db_pwd.c_str());
		g_conn->setSchema("net_db");

		PlayerModule::instance().bind(g_conn);

		NetLib::instance().run();
	}
	// server
	void onReciveServerHandle(uint64 serverId, uint64 sessionId, ByteBuffer& data)
	{
		uint16 cmd = data.read<uint16>();
		FuncHandle handle = g_handles[cmd];
		if (handle)
			handle(sessionId, m_pServer, data);
		else
			sLog.outWarning("warning can't find cmd:%d handle", cmd);
	}

	void onErrorServerHandle(IServer *server, const boost::system::error_code& error)
	{
		uint64 serverId = server->getId();
		uint16 port = server->getPort();
		NetLib::instance().destroyServer(serverId);
		sLog.outError("server error serverId:%lld port: %d error:%s \n", serverId, port, error.message().c_str());
	}

	// client
	void onReciveMasterHandle(uint64 clientId, ByteBuffer& data)
	{
		uint16 cmd = data.read<uint16>();
		sLog.outMessage("[GateServer::onReciveServerHandle] cmd: %d", cmd);
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
	IClient *m_pMaster;
};

NETWORK_END

	int main(int argc, char** argv)
{
	USE_NETWORK;
	DBServer app;
	app.run();
	return 0;
}