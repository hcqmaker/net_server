#include <cstdlib>
#include <deque>
#include <iostream>

#include "net.hpp"
#include "base.hpp"
#include "rule.hpp"

#include "function_handle.hpp"

#include <boost/lexical_cast.hpp>

using boost::asio::ip::tcp;

bool g_isDb = true;
USE_NETWORK;

CallBack g_CallBack[RULE_NUM] = {0};

class chat_client : public NetClient
{
public:
	chat_client(boost::asio::io_service& ioservice, const std::string& host, uint16 port)
		: NetClient(ioservice, host, port)
	{
		this->bindReceiveHandle(boost::bind(&chat_client::onReciveHandle, this, _1, _2));
		this->bindErrorHandle(boost::bind(&chat_client::onErrorHandle, this, _1, _2));

		g_CallBack[L2C_LOGIN] = &CALLBACK_L2C_LOGIN;
	}

	void onReciveHandle(uint64 clientId, ByteBuffer& data)
	{
		//data.read_skip(4);
		if (!g_isDb)
			Crypto::decrypt(data.rdata(), data.rsize(), data.rdata(), data.rsize());
		uint16 cmd = data.read<uint16>();
		CallBack handle = g_CallBack[cmd];
		if (handle)
			handle(data);
	}

	virtual void onErrorHandle(IClient *client, const boost::system::error_code& error)
	{
		uint64 clientId = client->getId();
		std::string host = client->getHost();
		uint16 port = client->getPort();
		sLog.outError("client err id: %lld host:%s port:%d error:%s", clientId, host.c_str(), port, error.message().c_str());
	}

};

void command_send(chat_client& c,  ByteBuffer& data, bool code = false)
{
	uint16 cmd;
	data >> cmd;

	if (code) // cmd is short
		Crypto::encrypt(data.data(), data.size(), data.data(), data.size());

	ByteBuffer buffer;
	buffer.append((uint32)data.size());
	buffer.append(data.data(), data.size());
	
	c.write(buffer);
}

void command(chat_client& c)
{
	const int MAX_BODY = 128;
	char line[MAX_BODY + 1];

	while (std::cin.getline(line, MAX_BODY + 1))
	{
		if (memcmp(line, "quit", 4) == 0)
			break;
		else if (memcmp(line, "login", 4) == 0)
		{
			g_isDb = false;
			PlayerLoginInfo info;
			std::cout<<"input uname:";
			std::cin>>info.user_name;
			std::cout<<"input upwd:";
			std::cin>>info.user_pwd;

			ByteBuffer data;
			data.append((uint16)C2L_LOGIN);
			data.append((uint8*)&info, sizeof(PlayerLoginInfo));
			command_send(c, data, true);
		}
	}
}

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
		command(c);

		c.close();
		t.join();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}