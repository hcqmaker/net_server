#include <cstdlib>
#include <deque>
#include <iostream>

#include "net.hpp"
#include "base.hpp"
#include "rule.hpp"

#include "function_handle.hpp"
#include "login_handle.hpp"

#include <boost/lexical_cast.hpp>

using boost::asio::ip::tcp;

bool g_isDb = true;
USE_NETWORK;

HandleManager g_HandleMgr;

login_handle g_login;

class chat_client : public NetClient
{
public:
	chat_client(boost::asio::io_service& ioservice, const std::string& host, uint16 port)
		: NetClient(ioservice, host, port)
	{
		this->bindReceiveHandle(boost::bind(&chat_client::onReciveHandle, this, _1, _2));
		this->bindErrorHandle(boost::bind(&chat_client::onErrorHandle, this, _1, _2));

		g_login.init(g_HandleMgr);
	}

	void onReciveHandle(uint64 clientId, ByteBuffer& data)
	{
		Crypto::decrypt(data.rdata(), data.rsize(), data.rdata(), data.rsize());
		g_HandleMgr.onReceive(data);
	}

	virtual void onErrorHandle(IClient *client, const boost::system::error_code& error)
	{
		uint64 clientId = client->getId();
		std::string host = client->getHost();
		uint16 port = client->getPort();
		sLog.outError("client err id: %lld host:%s port:%d error:%s", clientId, host.c_str(), port, error.message().c_str());
	}

};


int main(int argc, char* argv[])
{
	try
	{

		Config cfg;
		cfg.loadIni("option.ini");
		int port = cfg.getInt("port");
		std::string host = cfg.getString("host");

		boost::asio::io_service io_service;
		chat_client c(io_service, host.c_str(), port);

		boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));

		const int MAX_BODY = 128;
		char line[MAX_BODY + 1];

		while (std::cin.getline(line, MAX_BODY + 1))
		{
			if (memcmp(line, "quit", 4) == 0)
				break;
			g_HandleMgr.onSend(line, &c);
		}

		c.close();
		t.join();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}