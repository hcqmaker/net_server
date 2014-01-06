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

		ClientServerHandle tmpClientServerHandle(this);
		GameServerHandle tmpGameServerHandle(this);

		m_pCServer->bindReceiveHandle(boost::bind(&ClientServerHandle::onReciveServerHandle, &tmpClientServerHandle, _1, _2, _3));
		m_pCServer->bindErrorHandle(boost::bind(&ClientServerHandle::onErrorServerHandle, &tmpClientServerHandle, _1, _2));

		m_pGServer->bindReceiveHandle(boost::bind(&GameServerHandle::onReciveServerHandle, &tmpGameServerHandle, _1, _2, _3));
		m_pGServer->bindErrorHandle(boost::bind(&GameServerHandle::onErrorServerHandle, &tmpGameServerHandle, _1, _2));

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


		m_pMaster->bindReceiveHandle(boost::bind(&MasterHandle::onReciveClientHandle, &tmpMasterHandle, _1, _2));
		m_pMaster->bindErrorHandle(boost::bind(&MasterHandle::onErrorClientHandle, &tmpMasterHandle, _1, _2));

		m_pLogin->bindReceiveHandle(boost::bind(&LoginHandle::onReciveClientHandle, &tmpLoginHandle, _1, _2));
		m_pLogin->bindErrorHandle(boost::bind(&LoginHandle::onErrorClientHandle, &tmpLoginHandle, _1, _2));

		m_pChat->bindReceiveHandle(boost::bind(&ChatHandle::onReciveClientHandle, &tmpChatHandle, _1, _2));
		m_pChat->bindErrorHandle(boost::bind(&ChatHandle::onErrorClientHandle, &tmpChatHandle, _1, _2));

		NetLib::instance().run();
	}
	

NETWORK_END