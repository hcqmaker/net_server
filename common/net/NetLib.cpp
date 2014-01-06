#include "NetLib.hpp"
#include "NetClient.hpp"
#include "NetServer.hpp"

NETWORK_BEGIN

	//-----------------------------------------------------
	//
	NetLib::NetLib()
	{
	}

	//-----------------------------------------------------
	//
	NetLib::~NetLib()
	{
		ClientMap::iterator i = m_mapClient.begin();
		for (; i != m_mapClient.end(); ++i)
			delete i->second;
		m_mapClient.clear();

		ServerMap::iterator j = m_mapServer.begin();
		for (; j != m_mapServer.end(); ++j)
			delete j->second;
		m_mapServer.clear();
	}

	//-----------------------------------------------------
	//
	IClient* NetLib::createClient(const std::string& host, uint16 port)
	{
		WriteLock ulock(m_mutexClient);
		IClient *p = new NetClient(m_io_service, host, port);
		m_mapClient[p->getId()] = p;
		return p;
	}


	//-----------------------------------------------------
	//
	IServer* NetLib::createServer(uint16 port)
	{
		WriteLock ulock(m_mutexServer);
		tcp::endpoint endpoint(tcp::v4(), port);
		IServer *p = new NetServer(m_io_service, endpoint);
		m_mapServer[p->getId()] = p;
		return p;
	}

	//-----------------------------------------------------
	//
	IClient* NetLib::findClient(uint64 id)
	{
		ClientMap::iterator i = m_mapClient.find(id);
		if (i == m_mapClient.end())
			return NULL;
		return i->second;
	}

	//-----------------------------------------------------
	//
	IServer* NetLib::findServer(uint64 id)
	{
		ServerMap::iterator i = m_mapServer.find(id);
		if (i == m_mapServer.end())
			return NULL;
		return i->second;
	}

	//-----------------------------------------------------
	//
	bool NetLib::destroyClient(uint64 id)
	{
		WriteLock ulock(m_mutexClient);
		ClientMap::iterator i = m_mapClient.find(id);
		if (i == m_mapClient.end())
			return false;
		IClient *p = i->second;
		m_mapClient.erase(i);
		delete p;
		return true;
	}

	//-----------------------------------------------------
	//
	static void threadHandle(boost::asio::io_service *service)
	{
		service->run();
	}
	
	//-----------------------------------------------------
	//
	bool NetLib::destroyServer(uint64 id)
	{
		WriteLock ulock(m_mutexServer);
		ServerMap::iterator i = m_mapServer.find(id);
		if (i == m_mapServer.end())
			return false;
		IServer *p = i->second;
		m_mapServer.erase(i);
		delete p;
		return true;
	}


	//-----------------------------------------------------
	//
	void NetLib::run(int thread_num)
	{
		boost::thread_group tmpGroup;
		
		for (int i = 0; i < thread_num; ++i)
			tmpGroup.create_thread(boost::bind(threadHandle, &m_io_service));

		char command[128] = {0};
		while(std::cin.getline(command, 128))
		{
			if (memcmp(command, "quit", 4) == 0)
				break;
		}

		m_io_service.stop();

		//tmpGroup.join_all();
	}

	//-----------------------------------------------------
	//
	void NetLib::runOne()
	{
		m_io_service.run_one();
	}


NETWORK_END