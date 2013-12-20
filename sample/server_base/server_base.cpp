#include <iostream>

#include "Singleton.hpp"
#include "Log.hpp"
#include "ICommon.hpp"
#include "NetLib.hpp"
#include "NetQueue.hpp"
#include <boost/thread.hpp>

using namespace std;


NETWORK_BEGIN
class ServerApp : public IServerHandle, public IClientHandle
{
public:
	ServerApp()
		: m_pClient(0)
		, m_pServer(0)
	{

	}

	~ServerApp()
	{

	}

	void run()
	{
		m_pClient = NetLib::instance().createClient("127.0.0.1", 8010);
		m_pServer = NetLib::instance().createServer(8007);

		m_pServer->bindHandle(this);
		m_pClient->bindHandle(this);

		const int requestNum = 3;
		const int responseNum = 3;

		boost::thread_group tmpGroup;
		for(int i = 0; i < requestNum; ++i)
			tmpGroup.create_thread(boost::bind(&ServerApp::requestThreadHandle, this));
		for (int i = 0; i < responseNum; ++i)
			tmpGroup.create_thread(boost::bind(&ServerApp::responseThreadHandle, this));

		NetLib::instance().run();
		tmpGroup.join_all();
	}
	// server
	void onReciveServerHandle(long serverId, long sessionId, ByteBuffer& data)
	{
		uint16 cmd = data.read<uint16>();
		sLog.outMessage("[ServerApp::onReciveServerHandle] cmd: %d", cmd);
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
		uint16 cmd = data.read<uint16>();
		sLog.outMessage("[ServerApp::onReciveServerHandle] cmd: %d", cmd);
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
	ServerApp app;
	app.run();
	return 0;
}