#ifndef __NET_SESSION_HPP__
#define __NET_SESSION_HPP__

#include "Define.hpp"
#include "ICommon.hpp"

#include <deque>

#include <boost/asio.hpp>
#include <boost/smart_ptr/detail/atomic_count.hpp>


NETWORK_BEGIN
	
	class NetSession : public ISession, public boost::enable_shared_from_this<NetSession>
	{
	public:
		NetSession(boost::asio::io_service& io_service, long serverId);
		virtual ~NetSession();
		virtual long getServerId() { return m_nServerId; }

		virtual void start();
		virtual bool isConnected() { return m_bClosed; }
		virtual void write(const ByteBuffer& data);
		
		tcp::socket& socket() { return m_socket; }

	protected:
		void handle_read_header(const boost::system::error_code& error);
		void handle_read_body(const boost::system::error_code& error);
		void handle_write(const boost::system::error_code& error);

		void write_data();
		bool decode_header();

		void throwError(const boost::system::error_code& error);

		tcp::socket m_socket;
		long		m_nServerId;
		bool		m_bClosed;
		
		ByteBuffer	m_read;
		ByteBuffer m_wirte_bytes;
		BufferQueue m_write_queue;

		char m_cHead[HEAD_LENGTH];
		int m_nHeadCount;
	};

NETWORK_END

#endif // __NET_SESSION_HPP__