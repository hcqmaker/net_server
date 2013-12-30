#include "NetClient.hpp"
#include <boost/smart_ptr/detail/atomic_count.hpp>
#include <boost/lexical_cast.hpp>

NETWORK_BEGIN

	NetClient::NetClient(boost::asio::io_service& ioservice, const std::string& host, int port)
	 : m_strHost(host)
	 , m_nPort(port)
	 , m_nHeadCount(0)
	 , m_bClosed(true)
	 , m_io_service(ioservice)
	 ,m_socket(ioservice)
	{
		static boost::detail::atomic_count m_last_id(0l);
		m_nId = ++m_last_id;
		
		std::string s = boost::lexical_cast<std::string>(m_nPort);

		tcp::resolver resolver(m_io_service);
		tcp::resolver::query query(m_strHost.c_str(), s.c_str());
		tcp::resolver::iterator iterator = resolver.resolve(query);

		boost::asio::async_connect(m_socket, iterator,
			boost::bind(&NetClient::handle_connect, this,
			boost::asio::placeholders::error));
	}

	//-------------------------------------------------------------
	//
	NetClient::~NetClient()
	{
		do_close();
	}

	//-------------------------------------------------------------
	//
	void NetClient::close()
	{
		m_io_service.post(boost::bind(&NetClient::do_close, this));
	}

	//-------------------------------------------------------------
	//
	bool NetClient::isConnected()
	{
		return !m_bClosed;
	}

	//-------------------------------------------------------------
	//
	void NetClient::start()
	{
		boost::asio::async_read(m_socket,
			boost::asio::buffer(m_cHead, HEAD_LENGTH),
			boost::bind(&NetClient::handle_read_header, this,
			boost::asio::placeholders::error));
	}

	//-------------------------------------------------------------
	//
	void NetClient::write(const ByteBuffer& data)
	{
		bool is_empty = !m_write_queue.empty();
		m_write_queue.push_back(data);
		if (!is_empty)
			write_data();
	}

	//-------------------------------------------------------------
	//
	void NetClient::handle_connect(const boost::system::error_code& error)
	{
		if (!error)
		{
			start();
		}
		else
		{
			throwError(error);
		}
	}

	//-------------------------------------------------------------
	//
	void NetClient::handle_read_header(const boost::system::error_code& error)
	{
		if (!error && decode_header())
		{
			m_read.resize(m_nHeadCount);
			boost::asio::async_read(m_socket,
				boost::asio::buffer(m_read.bytes(), m_nHeadCount),
				boost::bind(&NetClient::handle_read_body, this,
				boost::asio::placeholders::error));
		}
		else
		{
			throwError(error);
		}
	}

	//-------------------------------------------------------------
	//
	void NetClient::handle_read_body(const boost::system::error_code& error)
	{
		if (!error)
		{
			m_nHeadCount = 0;
			memset(m_cHead, 0, 4);
			if (m_pHandle)
				m_pHandle->onReciveClientHandle(m_nId, m_read);
			start();
		}
		else
		{
			throwError(error);
		}
	}

	//-------------------------------------------------------------
	//
	void NetClient::handle_write(const boost::system::error_code& error)
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

	//-------------------------------------------------------------
	//
	void NetClient::throwError(const boost::system::error_code& error)
	{
		if (m_pHandle)
			m_pHandle->onErrorClientHandle(this, error);
	}

	//-------------------------------------------------------------
	//
	void NetClient::write_data()
	{
		const ByteBuffer& data = m_write_queue.front();
		m_wirte_bytes.clear();
		m_wirte_bytes.append((uint32)data.size());
		m_wirte_bytes.append(data.data(), data.size());

		boost::asio::async_write(m_socket,
			boost::asio::buffer(m_wirte_bytes.bytes()),
			boost::bind(&NetClient::handle_write, this,
			boost::asio::placeholders::error));
	}
	
	bool NetClient::decode_header()
	{
		std::memcpy((char*)&m_nHeadCount, m_cHead, sizeof(size_t));
		EndianConvertReverse(m_nHeadCount);
		if (m_nHeadCount < MAX_PACKET)
			return true;
		return false;
	}
	
	//-------------------------------------------------------------
	//
	void NetClient::do_close()
	{
		if (m_socket.is_open())
			m_socket.close();
	}
	
NETWORK_END