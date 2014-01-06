
#include <iostream>

#include "net.hpp"
#include "base.hpp"

#include "rule.hpp"

#include <boost/thread.hpp>

using namespace std;


NETWORK_BEGIN

	typedef std::map<uint64,PlayerLoginInfo> LoginInfoMap;

class LoginServer : public IServerHandle, public IClientHandle
{
public:
	LoginServer()
		: m_pClient(0)
		, m_pServer(0)
	{

	}

	~LoginServer()
	{

	}

	void run()
	{

		Config cfg;
		cfg.loadIni("option.ini");
		
		std::string db_host = cfg.getString("db_host");
		int db_port = cfg.getInt("db_port");

		int listener_port = cfg.getInt("listener_port");

		m_pClient = NetLib::instance().createClient(db_host.c_str(), db_port);
		m_pServer = NetLib::instance().createServer(listener_port);

		m_pServer->bindHandle(this);
		m_pClient->bindHandle(this);

		NetLib::instance().run();
	}
	// server
	void onReciveServerHandle(uint64 serverId, uint64 sessionId, ByteBuffer& data)
	{
		uint16 cmd = data.read<uint16>();

		if (cmd == C2L_LOGIN)
		{
			PlayerLoginInfo info;
			data.read((uint8*)&info, sizeof(PlayerLoginInfo));

			m_LoginInfoMap[sessionId] = info;
			
			ByteBuffer buffer;
			buffer << (uint16)L2D_PLAYER_INFO;
			buffer << (uint8)DB_PLAYER_NAME;
			buffer << (uint64)sessionId;
			data.append(info.user_name, MAX_NAME);
			m_pClient->write(buffer);
		}
	}

	void onErrorServerHandle(IServer *server, const boost::system::error_code& error)
	{
		uint64 serverId = server->getId();
		uint16 port = server->getPort();

		NetLib::instance().destroyServer(serverId);
		sLog.outError("server error serverId:%ld port: %d error:%s \n", serverId, port, error.message().c_str());
	}

	// client
	void onReciveClientHandle(uint64 clientId, ByteBuffer& data)
	{
		uint16 cmd = data.read<uint16>();
		if (cmd == D2L_PLAYER_INFO)
		{
			uint64 sessionId;
			uint8 tp;

			data >> sessionId;
			data >> tp;

			ByteBuffer buffer;
			buffer << (uint16)L2C_LOGIN;
			buffer << sessionId;

			if (tp == DB_SUCC)
			{
				PlayerInfo info;
				data.read((uint8*)&info, sizeof(PlayerInfo));
				buffer.append((uint8*)&info, sizeof(PlayerInfo));
				//buffer.append(data.rdata(), data.rsize());
				LoginInfoMap::iterator i = m_LoginInfoMap.find(sessionId);
				if (i == m_LoginInfoMap.end())
				{
					buffer << DB_ERR;
				}
				else
				{
					PlayerLoginInfo linfo = i->second;
					if (memcmp(info.user_pwd, linfo.user_pwd, MAX_NAME) == 0)
					{
						buffer << DB_SUCC;
						buffer.append((uint8*)&info, sizeof(PlayerInfo));
					}
					m_LoginInfoMap.erase(i);
				}
			}
			else
			{
				buffer << DB_ERR;
			}
			m_pServer->sendTo(sessionId, buffer);
		}
	}

	void onErrorClientHandle(IClient *client, const boost::system::error_code& error)
	{
		uint64 clientId = client->getId();
		std::string host = client->getHost();
		int port = client->getPort();

		NetLib::instance().destroyClient(clientId);
		sLog.outError("client error clientId:%ld host:%s port:%d error:%s \n", clientId, host.c_str(), port, error.message().c_str());

	}

protected:
	IServer *m_pServer;
	IClient *m_pClient;

	LoginInfoMap m_LoginInfoMap;
};


NETWORK_END


int main(int argc, char ** argv)
{
	USE_NETWORK;
	LoginServer app;
	app.run();

	return 0;
};