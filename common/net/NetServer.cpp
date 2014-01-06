#include "NetServer.hpp"
#include "NetSession.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

typedef boost::shared_lock<boost::shared_mutex> ReadLock;  
typedef boost::unique_lock<boost::shared_mutex> WriteLock;

NETWORK_BEGIN
	NetServer::NetServer(boost::asio::io_service& ioservice, const tcp::endpoint& endpoint)
	: m_endpoint(endpoint)
	, m_io_service(ioservice)
	, m_acceptor(ioservice, endpoint)
	{
		static boost::detail::atomic_count m_last_id(0l);
		m_nId = ++m_last_id;

		m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		start_accept();
	}

	//-------------------------------------------------------------
	//
	NetServer::~NetServer()
	{
		clear();
	}

	//-------------------------------------------------------------
	//
	void NetServer::sendToAll(const ByteBuffer& data)
	{
		ISessionPtrMap::iterator i = m_mapSession.begin();
		for (; i != m_mapSession.end(); ++i)
			i->second->write(data);
	}

	//-------------------------------------------------------------
	//
	void NetServer::sendTo(uint64 sessionId, const ByteBuffer& data)
	{
		ISessionPtrMap::iterator i = m_mapSession.find(sessionId);
		if (i != m_mapSession.end())
			i->second->write(data);
	}

	//-------------------------------------------------------------
	//
	bool NetServer::closeSession(uint64 sessionId)
	{
		WriteLock tmpLock(m_mutex);
		ISessionPtrMap::iterator i = m_mapSession.find(sessionId);
		if (i != m_mapSession.end())
		{
			m_mapSession.erase(i);
			return true;
		}
		return false;
	}

	//-------------------------------------------------------------
	//
	uint16 NetServer::getPort()
	{
		return m_endpoint.port();
	}

	//-------------------------------------------------------------
	//
	void NetServer::close()
	{
		m_io_service.stop();
	}

	//-------------------------------------------------------------
	//
	void NetServer::run()
	{
		m_io_service.run();
	}

	//-------------------------------------------------------------
	//
	void NetServer::runOne()
	{
		m_io_service.run_one();
	}

	//-------------------------------------------------------------
	//
	void NetServer::clear()
	{
		WriteLock tmpLock(m_mutex);
		m_mapSession.clear();
	}

	//-------------------------------------------------------------
	//
	void NetServer::start_accept()
	{
		NetSession *s = NULL;
		ISessionPtr ptr(s = new NetSession(m_io_service, m_nId));
		m_acceptor.async_accept(s->socket(),
			boost::bind(&NetServer::handle_accept, this, ptr,
			boost::asio::placeholders::error));
	}

	//-------------------------------------------------------------
	//
	void NetServer::handle_accept(ISessionPtr session, const boost::system::error_code& error)
	{
		if (!error)
		{
			WriteLock ulock(m_mutex);
			m_mapSession[session->getId()] = session;
			session->bindReceiveHandle(boost::bind(&NetServer::onReciveHandle, this, _1, _2));
			session->bindErrorHandle(boost::bind(&NetServer::onErrorHandle, this, _1, _2));
			session->start();
		}
		else
		{
			throwError(error);
		}

		start_accept();
	}

	//-------------------------------------------------------------
	//
	void NetServer::throwError(const boost::system::error_code& error)
	{
		if (m_hError)
			m_hError(this, error);
	}

	//-------------------------------------------------------------
	//
	void NetServer::onReciveHandle(uint64 sessionId, ByteBuffer& data)
	{
		if (m_hReceive)
			m_hReceive(m_nId, sessionId, data);
	}

	//-------------------------------------------------------------
	//
	void NetServer::onErrorHandle(ISessionPtr session, const boost::system::error_code& error)
	{
		uint64 sessionId = session->getId();
		this->closeSession(sessionId);
		sLog.outError("session error session:%lld server:%lld error:%s \n", sessionId, this->m_nId, error.message().c_str());
	}

NETWORK_END