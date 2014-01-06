#ifndef __NET_LIB_HPP__
#define __NET_LIB_HPP__

#include "base/Define.hpp"
#include "base/Singleton.hpp"
#include "net/ICommon.hpp"

#include <map>

NETWORK_BEGIN
	typedef std::map<uint64, IServer*> ServerMap;
	typedef std::map<uint64, IClient*> ClientMap;

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

		IClient* createClient(const std::string& host, uint16 port);
		IServer* createServer(uint16 port);

		IClient* findClient(uint64 id);
		IServer* findServer(uint64 id);

		bool destroyClient(uint64 id);
		bool destroyServer(uint64 id);

		void run(int thread_num = 1);
		void runOne();

	};

NETWORK_END

#endif // __NET_LIB_HPP__