#include <iostream>

#include "net.hpp"
#include "base.hpp"
#include <boost/thread.hpp>

using namespace std;


NETWORK_BEGIN
class GateServer : public IServerHandle, public IClientHandle
{
public:
	GateServer()
		: m_pClient(0)
		, m_pServer(0)
	{

	}

	~GateServer()
	{

	}

	void run()
	{

		Config cfg;
		cfg.loadIni("option.ini");
		int game_port = cfg.getInt("game_port");
		std::string game_host = cfg.getString("game_host");

		int listener_port = cfg.getInt("listener_port");

		int request_threadnum = cfg.getInt("request_threadnum");
		int response_threadnum = cfg.getInt("response_threadnum");

		m_pClient = NetLib::instance().createClient(game_host.c_str(), game_port);
		m_pServer = NetLib::instance().createServer(listener_port);

		m_pServer->bindHandle(this);
		m_pClient->bindHandle(this);


		boost::thread_group tmpGroup;
		for(int i = 0; i < request_threadnum; ++i)
			tmpGroup.create_thread(boost::bind(&GateServer::requestThreadHandle, this));
		for (int i = 0; i < response_threadnum; ++i)
			tmpGroup.create_thread(boost::bind(&GateServer::responseThreadHandle, this));

		NetLib::instance().run();
		tmpGroup.join_all();
	}
	// server
	void onReciveServerHandle(long serverId, long sessionId, ByteBuffer& data)
	{
		Crypto::decrypt(data.data(), data.size(), data.data(), data.size());
		uint16 cmd = data.read<uint16>();
		sLog.outMessage("[GateServer::onReciveServerHandle] cmd: %d", cmd);

		Crypto::encrypt(data.data(), data.size(), data.data(), data.size());
		m_pServer->sendTo(sessionId, data);

	}

	void onErrorServerHandle(IServer *server, const boost::system::error_code& error)
	{
		long serverId = server->getId();
		NetLib::instance().destroyServer(serverId);
		sLog.outError("server error serverId:%ld error:%s \n", serverId, error.message().c_str());
	}

	// client
	void onReciveClientHandle(long clientId, ByteBuffer& data)
	{
		Crypto::decrypt(data.data(), data.size(), data.data(), data.size());
		uint16 cmd = data.read<uint16>();
		sLog.outMessage("[GateServer::onReciveServerHandle] cmd: %d", cmd);
	}

	void onErrorClientHandle(IClient *client, const boost::system::error_code& error)
	{
		long clientId = client->getId();
		NetLib::instance().destroyClient(clientId);
		sLog.outError("client error clientId:%ld error:%s \n", clientId, error.message().c_str());
	}

	void responseThreadHandle()
	{

	}

	void requestThreadHandle()
	{

	}

protected:
	IServer *m_pServer;
	IClient *m_pClient;
};

NETWORK_END

int main(int argc, char** argv)
{
	USE_NETWORK;
	GateServer app;
	app.run();
	return 0;
}