#ifndef __NET_LIB_HPP__
#define __NET_LIB_HPP__

#include "Define.hpp"
#include "ICommon.hpp"
#include "Singleton.hpp"
#include <map>

NETWORK_BEGIN
	typedef std::map<long, IServer*> ServerMap;
	typedef std::map<long, IClient*> ClientMap;

	class NetLib : public Singleton<NetLib>
	{
	private:
		NetLib();
		~NetLib();
		friend class Singleton<NetLib>;

		ServerMap m_mapServer;
		ClientMap m_mapClient;

		boost::shared_mutex m_mutexServer;
		boost::shared_mutex m_mutexClient;

		boost::asio::io_service m_io_service;
	public:

		IClient* createClient(const std::string& host, int port);
		IServer* createServer(int port);

		IClient* findClient(long id);
		IServer* findServer(long id);

		bool destroyClient(long id);
		bool destroyServer(long id);

		void run(int thread_num = 2);
		void runOne();

	};

NETWORK_END

#endif // __NET_LIB_HPP__