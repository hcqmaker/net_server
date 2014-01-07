#ifndef __LOGIN_HANDLE_HPP__
#define __LOGIN_HANDLE_HPP__

#include "rule.hpp"
#include "base.hpp"
#include "net.hpp"
#include "function_handle.hpp"

USE_NETWORK;


class login_handle
{
public:
	login_handle(){}
	void init(HandleManager &g)
	{
		g.registSend("lg_in", boost::bind(&login_handle::sendLogin, this, _1));
		g.registerReceive(L2C_LOGIN, boost::bind(&login_handle::receiveLogin, this, _1));

		g.registSend("lg_cr", boost::bind(&login_handle::sendCreate, this, _1));
		g.registerReceive(L2C_CREATE, boost::bind(&login_handle::receiveCreate, this, _1));
	}

	void sendLogin(NetClient *c)
	{
		PlayerLoginInfo info;
		std::cout<<"input uname:";
		std::cin>>info.user_name;
		std::cout<<"input upwd:";
		std::cin>>info.user_pwd;

		ByteBuffer data;
		data.append((uint16)C2L_LOGIN);
		data.append((uint8*)&info, sizeof(PlayerLoginInfo));
		command_send(c, data);
	}

	void receiveLogin(ByteBuffer &data)
	{
		uint8 tp;
		data >> tp;

		if (tp == DB_SUCC)
		{
			PlayerInfo info;
			data.read((uint8*)&info, sizeof(PlayerInfo));
			sLog.outMessage("\n uid:%d\n user_name:%s\n user_pwd:%s\n level:%d\n gender:%d\n role_id:%d\n scene_id:%d\n x:%d\n y:%d", 
				info.user_id, info.user_name, info.user_pwd, info.level, info.gender, info.role_id, info.scene_id, info.x, info.y);
		}

		if (tp == DB_ERR)
		{
			sLog.outMessage("login fail uname or upwd is wrong");
		}

		if (tp == DB_EMPT)
		{
			sLog.outMessage("can't find uname");
		}
	}

	void sendCreate(NetClient *c)
	{
		PlayerInfo info;
		std::cout <<"input uname:";
		std::cin >> info.user_name;
		std::cout << "input upwd:";
		std::cin >> info.user_pwd;

		ByteBuffer data;
		data.append((uint16)C2L_CREATE);
		data.append((uint8*)&info, sizeof(PlayerInfo));
		command_send(c, data);
	}

	void receiveCreate(ByteBuffer &data)
	{
		uint8 tp;
		data >> tp;

		if (tp == DB_SUCC)
		{
			sLog.outMessage("create successfull");
		}
		else
		{
			sLog.outMessage("create fail");
		}
	}
};



#endif // __LOGIN_HANDLE_HPP__