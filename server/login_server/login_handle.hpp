#ifndef __LOGIN_HANDLE_HPP__
#define __LOGIN_HANDLE_HPP__


#include "net.hpp"
#include "base.hpp"

#include "rule.hpp"


USE_NETWORK;

typedef std::map<PlayerId,PlayerLoginInfo> LoginInfoMap;

typedef void (*SessionHandle)(IClient *db, uint64 serverId, uint64 sessionId, ByteBuffer& data);
typedef void (*ClientHandle)(IServer *s, uint64 clientId, ByteBuffer& data);

LoginInfoMap g_LoginInfoMap;

void FUNCTION_C2L_LOGIN(IClient *db, uint64 serverId, uint64 sessionId, ByteBuffer& data)
{
	uint64 c_sessionId;
	data >> c_sessionId;

	PlayerLoginInfo info;
	data.read((uint8*)&info, sizeof(PlayerLoginInfo));

	PlayerId pid(c_sessionId, sessionId, serverId);
	g_LoginInfoMap[pid] = info;

	ByteBuffer buffer;
	buffer << (uint16)L2D_PLAYER_INFO;
	buffer << (uint8)DB_PLAYER_NAME;
	buffer.append((uint8*)&pid, sizeof(PlayerId));
	buffer.append(info.user_name, MAX_NAME);
	db->write(buffer);
}


void FUNCTION_D2L_PLAYER_INFO(IServer *s, uint64 clientId, ByteBuffer& data)
{
	PlayerId pid;
	uint8 tp;

	data.read((uint8*)&pid, sizeof(PlayerId));
	data >> tp;

	ByteBuffer buffer;
	buffer << (uint16)L2C_LOGIN;
	buffer << (uint64)pid.csessionId;

	if (tp == DB_SUCC)
	{
		PlayerInfo info;
		data.read((uint8*)&info, sizeof(PlayerInfo));
		LoginInfoMap::iterator i = g_LoginInfoMap.find(pid);
		if (i == g_LoginInfoMap.end())
		{
			buffer << (uint8)DB_ERR;
		}
		else
		{
			PlayerLoginInfo linfo = i->second;
			if (memcmp(info.user_pwd, linfo.user_pwd, MAX_PWD) == 0)
			{
				buffer << (uint8)DB_SUCC;
				buffer.append((uint8*)&info, sizeof(PlayerInfo));
			}
			g_LoginInfoMap.erase(i);
		}
	}
	else
	{
		buffer << tp;
	}
	s->sendTo(pid.lsessionId, buffer);
}


void FUNCTION_C2L_CREATE(IClient *db, uint64 serverId, uint64 sessionId, ByteBuffer& data)
{
	uint64 c_sessionId;
	data >> c_sessionId;

	PlayerId lgin(c_sessionId, sessionId, serverId);

	ByteBuffer buffer;
	buffer << (uint16)L2D_CREATE_PLAYER;
	buffer.append((uint8*)&lgin, sizeof(PlayerId));
	buffer.append(data.rdata(), data.rsize());
	db->write(buffer);
}

void FUNCTION_D2L_CREATE_PLAYER(IServer *s, uint64 clientId, ByteBuffer& data)
{
	PlayerId pid;
	data.read((uint8*)&pid, sizeof(PlayerId));

	ByteBuffer buffer;
	buffer << (uint16)L2C_CREATE;
	buffer << (uint64)pid.csessionId;
	buffer.append(data.rdata(), data.rsize());
	s->sendTo(pid.lsessionId, buffer);
}

void register_session(SessionHandle handle[])
{
	handle[C2L_LOGIN] = &FUNCTION_C2L_LOGIN;
	handle[C2L_CREATE] = &FUNCTION_C2L_CREATE;
}

void register_client(ClientHandle handle[])
{
	handle[D2L_PLAYER_INFO] = &FUNCTION_D2L_PLAYER_INFO;
	handle[D2L_CREATE_PLAYER] = &FUNCTION_D2L_CREATE_PLAYER;
}


#endif // __LOGIN_HANDLE_HPP__