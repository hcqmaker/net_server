
#include <iostream>

#include "net.hpp"
#include "base.hpp"

#include "rule.hpp"

#include <boost/thread.hpp>

using namespace std;


NETWORK_BEGIN

	typedef std::map<GroupLogin,PlayerLoginInfo> LoginInfoMap;

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

		NetLib::instance().run();
	}
	// server
	void onReciveServerHandle(uint64 serverId, uint64 sessionId, ByteBuffer& data)
	{
		uint16 cmd = data.read<uint16>();

		if (cmd == C2L_LOGIN)
		{
			uint64 c_sessionId;
			data >> c_sessionId;

			PlayerLoginInfo info;
			data.read((uint8*)&info, sizeof(PlayerLoginInfo));

			GroupLogin lgin(c_sessionId, sessionId, serverId);
			m_LoginInfoMap[lgin] = info;
			
			ByteBuffer buffer;
			buffer << (uint16)L2D_PLAYER_INFO;
			buffer << (uint8)DB_PLAYER_NAME;
			buffer.append((uint8*)&lgin, sizeof(GroupLogin));
			buffer.append(info.user_name, MAX_NAME);
			m_pDataBase->write(buffer);
		}
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
		if (cmd == D2L_PLAYER_INFO)
		{
			GroupLogin lgin;
			uint8 tp;

			data.read((uint8*)&lgin, sizeof(GroupLogin));
			data >> tp;

			ByteBuffer buffer;
			buffer << (uint16)L2C_LOGIN;
			buffer << (uint64)lgin.csessionId;

			if (tp == DB_SUCC)
			{
				PlayerInfo info;
				data.read((uint8*)&info, sizeof(PlayerInfo));
				LoginInfoMap::iterator i = m_LoginInfoMap.find(lgin);
				if (i == m_LoginInfoMap.end())
				{
					buffer << (uint8)DB_ERR;
				}
				else
				{
					PlayerLoginInfo linfo = i->second;
					if (memcmp(info.user_pwd, linfo.user_pwd, MAX_PWD) == 0)
					{
						buffer << (uint8)DB_SUCC;
						buffer.append((uint8*)&info, sizeof(PlayerInfo));
					}
					m_LoginInfoMap.erase(i);
				}
			}
			else
			{
				buffer << (uint8)DB_ERR;
			}
			m_pServer->sendTo(lgin.lsessionId, buffer);
		}
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