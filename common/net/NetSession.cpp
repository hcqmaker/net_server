#include "NetSession.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/lexical_cast.hpp>

NETWORK_BEGIN

	//-------------------------------------------------------------------------
	//
	NetSession::NetSession(boost::asio::io_service& io_service, uint64 serverId)
	: m_socket(io_service)
	, m_nServerId(serverId)
	, m_nHeadCount(0)
	{
		static boost::detail::atomic_count m_last_id(0l);
		m_nId = ++m_last_id;
	}

	//-------------------------------------------------------------------------
	//
	NetSession::~NetSession()
	{

	}

	//-------------------------------------------------------------------------
	//
	void NetSession::start()
	{
		boost::asio::async_read(m_socket,
			boost::asio::buffer(m_cHead, HEAD_LENGTH),
			boost::bind(&NetSession::handle_read_header, shared_from_this(),
			boost::asio::placeholders::error));
	}

	//-------------------------------------------------------------------------
	//
	void NetSession::write(const ByteBuffer& buffer)
	{
		bool is_empty = !m_write_queue.empty();
		m_write_queue.push_back(buffer);
		if (!is_empty)
			write_data();
	}

	//-------------------------------------------------------------------------
	//
	void NetSession::handle_read_header(const boost::system::error_code& error)
	{
		if (!error && decode_header())
		{
			m_read.resize(m_nHeadCount);
			boost::asio::async_read(m_socket,
				boost::asio::buffer(m_read.bytes(), m_nHeadCount),
				boost::bind(&NetSession::handle_read_body, shared_from_this(),
				boost::asio::placeholders::error));
		}
		else
		{
			throwError(error);
		}
	}

	//-------------------------------------------------------------------------
	//
	void NetSession::handle_read_body(const boost::system::error_code& error)
	{
		if (!error)
		{
			m_nHeadCount = 0;
			memset(m_cHead, 0, 4);
			if (m_hReceive)
				m_hReceive(m_nId, m_read);
			start();
		}
		else
		{
			throwError(error);
		}
	}

	//-------------------------------------------------------------------------
	//
	void NetSession::handle_write(const boost::system::error_code& error)
	{
		if (!error)
		{
			m_write_queue.pop_front();
			if (!m_write_queue.empty())
				write_data();
		}
		else
		{
			throwError(error);
		}
	}

	//-------------------------------------------------------------------------
	//
	void NetSession::write_data()
	{		
		const ByteBuffer& data = m_write_queue.front();
		m_wirte_bytes.clear();
		m_wirte_bytes.append((uint32)data.size());
		m_wirte_bytes.append(data.data(), data.size());

		boost::asio::async_write(m_socket,
			boost::asio::buffer(m_wirte_bytes.bytes()),
			boost::bind(&NetSession::handle_write, shared_from_this(),
			boost::asio::placeholders::error));
	}

	//-------------------------------------------------------------------------
	//
	bool NetSession::decode_header()
	{
		std::memcpy((char*)&m_nHeadCount, m_cHead, sizeof(size_t));
		EndianConvertReverse(m_nHeadCount);
		if (m_nHeadCount < MAX_PACKET)
			return true;
		return false;
	}

	//-------------------------------------------------------------------------
	//
	void NetSession::throwError(const boost::system::error_code& error)
	{
		if (m_hError)
			m_hError(shared_from_this(), error);
	}
NETWORK_END