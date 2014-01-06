#ifndef __NET_CLIENT_HPP__
#define __NET_CLIENT_HPP__

#include "base/Define.hpp"
#include "net/ICommon.hpp"

NETWORK_BEGIN
	class NetClient : public IClient
	{
	public:
		NetClient(boost::asio::io_service& ioservice, const std::string& host, uint16 port);
		~NetClient();

		virtual void close();
		virtual bool isConnected();
		virtual void write(const ByteBuffer& pack);

		virtual std::string getHost() { return m_strHost; }
		virtual uint16 getPort() { return m_nPort; }

	protected:
		void handle_connect(const boost::system::error_code& error);
		void handle_read_header(const boost::system::error_code& error);
		void handle_read_body(const boost::system::error_code& error);
		void handle_write(const boost::system::error_code& error);
		
		void throwError(const boost::system::error_code& error);

		void start();
		void write_data();
		bool decode_header();
		
		void do_close();
		
		std::string m_strHost;
		int m_nPort;
		
		ByteBuffer	m_read;
		BufferQueue m_write_queue;
		ByteBuffer m_wirte_bytes;
		
		char 	m_cHead[HEAD_LENGTH];
		int 	m_nHeadCount;
		bool	m_bClosed;
		
		tcp::socket m_socket;
		boost::asio::io_service& m_io_service;
	};
NETWORK_END


#endif // __NET_CLIENT_HPP__