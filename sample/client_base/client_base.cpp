#include <cstdlib>
#include <deque>
#include <iostream>

#include "Define.hpp"
#include "Packet.hpp"
#include "ICommon.hpp"
#include "NetClient.hpp"

#include <boost/lexical_cast.hpp>

using boost::asio::ip::tcp;

USE_NETWORK;


class chat_client : public NetClient, public IClientHandle 
{
public:
	chat_client(boost::asio::io_service& ioservice, const std::string& host, int port)
		: NetClient(ioservice, host, port)
	{
		this->bindHandle(this);
	}

	virtual void onReciveClientHandle(long clientId, ByteBuffer& data)
	{
		uint16 cmd = data.read<uint16>();
		size_t len = data.size() - data.rpos();
		uint8 s[128] = {0};
		data.read(s, len);
		sLog.outMessage("data %s", s);
	}

	virtual void onErrorClientHandle(IClient *client, const boost::system::error_code& error)
	{

	}

};

int main(int argc, char* argv[])
{
	try
	{

		boost::asio::io_service io_service;
		chat_client c(io_service, "127.0.0.1", 8007);

		boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));
		const int MAX_BODY = 128;
		char line[MAX_BODY + 1];
		while (std::cin.getline(line, MAX_BODY + 1))
		{
			if (memcmp(line, "quit", 4) == 0)
				break;

			using namespace std; // For strlen and memcpy.
			
			ByteBuffer buffer;
			buffer.append((uint16)(10));
			buffer.append(line, strlen(line));
			c.write(buffer);
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