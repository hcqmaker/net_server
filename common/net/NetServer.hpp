#ifndef __NET_SERVER_HPP__
#define __NET_SERVER_HPP__

#include "net/ICommon.hpp"
#include <boost/thread.hpp>
#include <boost/smart_ptr/detail/atomic_count.hpp>

NETWORK_BEGIN
	using boost::asio::ip::tcp;
	class NetServer : public IServer, public ISessionHandle
	{
	public:
		NetServer(boost::asio::io_service& ioservice, const tcp::endpoint& endpoint);
		virtual ~NetServer();
		virtual void sendToAll(const ByteBuffer& data);
		virtual void sendTo(long sessionId, const ByteBuffer& data);
		virtual bool closeSession(long sessionId);
		virtual void close();

		virtual void run();
		virtual void runOne();

		void clear();
		
		boost::asio::io_service& getService() { return m_io_service; }
	protected:
		void start_accept();
		void handle_accept(ISessionPtr session, const boost::system::error_code& error);
		void throwError(const boost::system::error_code& error);

		void onReciveSessionHandle(long sessionId, ByteBuffer& data);
		void onErrorSessionHandle(ISessionPtr session, const boost::system::error_code& error);

	private:
		long m_nId;
		tcp::endpoint m_endpoint;

		boost::asio::io_service& m_io_service;
		tcp::acceptor m_acceptor;
		ISessionPtrMap m_mapSession;
		boost::shared_mutex m_mutex;
	};


NETWORK_END

#endif // __NET_SERVER_HPP__