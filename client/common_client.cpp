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

class chat_client : public NetClient, public IClientHandle 
{
public:
	chat_client(boost::asio::io_service& ioservice, const std::string& host, uint16 port)
		: NetClient(ioservice, host, port)
	{
		this->bindHandle(this);


		g_CallBack[L2C_LOGIN] = &CALLBACK_L2C_LOGIN;
		g_CallBack[S2D_PLAYER_INFO] = &CALLBACK_S2D_PLAYER_INFO;
	}

	virtual void onReciveClientHandle(uint64 clientId, ByteBuffer& data)
	{
		data.read_skip(4);
		if (!g_isDb)
			Crypto::decrypt(data.rdata(), data.rsize(), data.rdata(), data.rsize());
		uint16 cmd = data.read<uint16>();
		CallBack handle = g_CallBack[cmd];
		if (handle)
			handle(data);
	}

	virtual void onErrorClientHandle(IClient *client, const boost::system::error_code& error)
	{
		sLog.outError("error:%s", error.message().c_str());
	}

};

void command_send(chat_client& c,  ByteBuffer& data, bool code = false)
{
	uint16 cmd;
	data >> cmd;

	if (code) // cmd is short
		Crypto::encrypt(data.rdata(), data.rsize(), data.rdata(), data.rsize());

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
			std::cin>>info.user_name>>info.user_pwd;

			ByteBuffer data;
			data.append((uint16)C2L_LOGIN);
			data.append((uint8*)&info, sizeof(PlayerLoginInfo));
			command_send(c, data);
		}
		else if (memcmp(line, "dbpinfo", 7) == 0) // player info
		{
			g_isDb = true;
			char name[MAX_NAME] = {0};
			std::cout<<"input user_name:";
			std::cin>>name;
			ByteBuffer data;
			data.append((uint16)S2D_PLAYER_INFO);
			data << (uint8)DB_PLAYER_NAME;
			data.append(name, MAX_NAME);
			command_send(c, data);
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