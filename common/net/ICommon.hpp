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

	// handle
	typedef boost::function<void(uint64 id, ByteBuffer& data)> ReceiveHandle;
	typedef boost::function<void(IClient *client, const boost::system::error_code& error)> ErrorClientHandle;
	typedef boost::function<void(ISessionPtr session, const boost::system::error_code& error)> ErrorSessionHandle;

	typedef boost::function<void(uint64 serverId, uint64 sessionId, ByteBuffer& data)> ReciveServerHandle;
	typedef boost::function<void(IServer *server, const boost::system::error_code& error)> ErrorServerHandle;

	using boost::asio::ip::tcp;
	
	class IClient
	{
	public:
		IClient():m_nId(0l){}
		virtual ~IClient(){}
		virtual uint64 getId() { return m_nId;}
		
		virtual void tryConnect() = 0;

		virtual void close() = 0;
		virtual bool isConnected() = 0;
		virtual void write(const ByteBuffer& pack) = 0;

		virtual std::string getHost() = 0;
		virtual uint16 getPort() = 0;

		void bindReceiveHandle(ReceiveHandle handle) { m_hReceive = handle; }
		void unBindReceiveHandle() { m_hReceive = NULL; }

		void bindErrorHandle(ErrorClientHandle handle) { m_hError = handle; }
		void unBindErrorHandle() { m_hError = NULL; }

	protected:
		uint64 m_nId;
		ReceiveHandle m_hReceive;
		ErrorClientHandle m_hError;
	};

	class ISession
	{
	public:
		ISession():m_nId(0l){}
		virtual ~ISession(){}

		virtual uint64 getServerId() = 0;
		virtual uint64 getId() { return m_nId;}

		virtual void start() = 0;
		virtual bool isConnected() = 0;
		virtual void write(const ByteBuffer& pack) = 0;

		void bindReceiveHandle(ReceiveHandle handle) { m_hReceive = handle; }
		void unBindReceiveHandle() { m_hReceive = NULL; }

		void bindErrorHandle(ErrorSessionHandle handle) { m_hError = handle; }
		void unBindErrorHandle() { m_hError = NULL; }

	protected:
		uint64 m_nId;
		ReceiveHandle m_hReceive;
		ErrorSessionHandle m_hError;
	};
	

	class IServer
	{
	public:
		IServer():m_nId(0l){}
		virtual ~IServer(){}
		virtual uint64 getId() { return m_nId;}

		virtual void sendToAll(const ByteBuffer& ptr) = 0;
		virtual void sendTo(uint64 sessionId, const ByteBuffer& ptr) = 0;
		virtual bool closeSession(uint64 sessionId) = 0;
		virtual void close() = 0;

		virtual uint16 getPort() = 0;

		void bindReceiveHandle(ReciveServerHandle handle) { m_hReceive = handle; }
		void unBindReceiveHandle() { m_hReceive = NULL; }

		void bindErrorHandle(ErrorServerHandle handle) { m_hError = handle; }
		void unBindErrorHandle() { m_hError = NULL; }

	protected:
		uint64 m_nId;
		ReciveServerHandle m_hReceive;
		ErrorServerHandle m_hError;
	};



NETWORK_END

#endif // __ICOMMON_HPP__