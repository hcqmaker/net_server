#include "GateServer.hpp"


NETWORK_BEGIN
	GateServer::GateServer()
		: m_pCServer(0)
		, m_pGServer(0)
		, m_pMaster(0)
		, m_pLogin(0)
	{ }

	GateServer::~GateServer() { }

	void GateServer::run()
	{
		Config cfg;
		cfg.loadIni("option.ini");

		// server
		int listener_port = cfg.getInt("listener_port");
		int listener_game_port = cfg.getInt("listener_game_port");

		m_pGServer = NetLib::instance().createServer(listener_game_port);
		m_pCServer = NetLib::instance().createServer(listener_port);

		GameServerHandle tmpGameServerHandle(this);
		ClientServerHandle tmpClientServerHandle(this);

		m_pCServer->bindHandle(&tmpClientServerHandle);
		m_pGServer->bindHandle(&tmpGameServerHandle);

		// client
		std::string master_host = cfg.getString("master_host");
		int master_port = cfg.getInt("master_port");

		std::string login_host = cfg.getString("login_host");
		int login_port = cfg.getInt("login_port");

		std::string chat_host = cfg.getString("chat_host");
		int chat_port = cfg.getInt("chat_port");

		m_pMaster = NetLib::instance().createClient(master_host.c_str(), master_port);
		m_pLogin = NetLib::instance().createClient(login_host.c_str(), login_port);
		m_pChat = NetLib::instance().createClient(chat_host.c_str(), chat_port);


		MasterHandle tmpMasterHandle(this);
		LoginHandle tmpLoginHandle(this);
		ChatHandle tmpChatHandle(this);

		m_pMaster->bindHandle(&tmpMasterHandle);
		m_pLogin->bindHandle(&tmpLoginHandle);
		m_pChat->bindHandle(&tmpChatHandle);

		NetLib::instance().run();
	}
	/*
	// server
	void onReciveServerHandle(uint64 serverId, uint64 sessionId, ByteBuffer& data)
	{
		Crypto::decrypt(data.data(), data.size(), data.data(), data.size());
		uint16 cmd = data.read<uint16>();
		sLog.outMessage("[GateServer::onReciveServerHandle] cmd: %d", cmd);

		Crypto::encrypt(data.data(), data.size(), data.data(), data.size());
		m_pCServer->sendTo(sessionId, data);
	}

	void onErrorServerHandle(IServer *server, const boost::system::error_code& error)
	{
		uint64 serverId = server->getId();
		NetLib::instance().destroyServer(serverId);
		sLog.outError("server error serverId:%ld error:%s \n", serverId, error.message().c_str());
	}

	// client
	void onReciveClientHandle(uint64 clientId, ByteBuffer& data)
	{
		Crypto::decrypt(data.data(), data.size(), data.data(), data.size());
		uint16 cmd = data.read<uint16>();
		sLog.outMessage("[GateServer::onReciveServerHandle] cmd: %d", cmd);
	}

	void onErrorClientHandle(IClient *client, const boost::system::error_code& error)
	{
		uint64 clientId = client->getId();
		NetLib::instance().destroyClient(clientId);
		sLog.outError("client error clientId:%ld error:%s \n", clientId, error.message().c_str());
	}
	*/

NETWORK_END