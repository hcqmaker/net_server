#ifndef __ICOMMON_HPP__
#define __ICOMMON_HPP__

#include "base/Define.hpp"
#include "base/LogDefine.hpp"
#include "net/ByteBuffer.hpp"

#include <deque>
#include <set>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>


typedef boost::shared_lock<boost::shared_mutex> ReadLock;  
typedef boost::unique_lock<boost::shared_mutex> WriteLock;



#define _WIN32_WINNT 0x0501

NETWORK_BEGIN
	
	class IClient;
	class ISession;
	class IServer;

	typedef boost::shared_ptr<ISession> ISessionPtr;
	typedef std::map<uint64, ISessionPtr> ISessionPtrMap;
	typedef std::deque<ByteBuffer> BufferQueue;

	typedef boost::shared_ptr<IClient> IClientPtr;
	typedef std::map<uint64, IClient> IClientPtrMap;

	using boost::asio::ip::tcp;
	
	class IClientHandle
	{
	public:
		virtual void onReciveClientHandle(uint64 clientId, ByteBuffer& data) = 0;
		virtual void onErrorClientHandle(IClient *client, const boost::system::error_code& error) = 0;
	};

	class ISessionHandle
	{
	public:
		virtual void onReciveSessionHandle(uint64 sessionId, ByteBuffer& data) = 0;
		virtual void onErrorSessionHandle(ISessionPtr session, const boost::system::error_code& error) = 0;
	};

	class IServerHandle
	{
	public:
		virtual void onReciveServerHandle(uint64 serverId, uint64 sessionId, ByteBuffer& data) = 0;
		virtual void onErrorServerHandle(IServer *server, const boost::system::error_code& error) = 0;
	};

	class IClient
	{
	public:
		IClient():m_nId(0l),m_pHandle(0){}
		virtual ~IClient(){}
		virtual uint64 getId() { return m_nId;}
		
		virtual void close() = 0;
		virtual bool isConnected() = 0;
		virtual void write(const ByteBuffer& pack) = 0;

		virtual std::string getHost() = 0;
		virtual uint16 getPort() = 0;

		void bindHandle(IClientHandle* handle) { m_pHandle = handle; }
		IClientHandle * unBindHandle() { IClientHandle *p = m_pHandle; m_pHandle = 0; return p; }

	protected:
		uint64 m_nId;
		IClientHandle *m_pHandle;
	};

	class ISession
	{
	public:
		ISession():m_nId(0l),m_pHandle(0){}
		virtual ~ISession(){}

		virtual uint64 getServerId() = 0;
		virtual uint64 getId() { return m_nId;}

		virtual void start() = 0;
		virtual bool isConnected() = 0;
		virtual void write(const ByteBuffer& pack) = 0;

		void bindHandle(ISessionHandle* handle) { m_pHandle = handle; }
		ISessionHandle * unBindHandle() { ISessionHandle *p = m_pHandle; m_pHandle = 0; return p; }

	protected:
		uint64 m_nId;
		ISessionHandle *m_pHandle;
	};
	

	class IServer
	{
	public:
		IServer():m_nId(0l),m_pHandle(0){}
		virtual ~IServer(){}
		virtual uint64 getId() { return m_nId;}

		virtual void sendToAll(const ByteBuffer& ptr) = 0;
		virtual void sendTo(uint64 sessionId, const ByteBuffer& ptr) = 0;
		virtual bool closeSession(uint64 sessionId) = 0;
		virtual void close() = 0;

		virtual uint16 getPort() = 0;

		void bindHandle(IServerHandle* handle) { m_pHandle = handle; }
		IServerHandle * unBindHandle() { IServerHandle *p = m_pHandle; m_pHandle = 0; return p; }

	protected:
		uint64 m_nId;
		IServerHandle *m_pHandle;
	};



NETWORK_END

#endif // __ICOMMON_HPP__