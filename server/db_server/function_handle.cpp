#include "function_handle.hpp"
#include "PlayerModule.hpp"



void HANDLE_L2D_PLAYER_INFO(uint64 sessionId, IServer *server, ByteBuffer& data)
{
	PlayerInfo *info = NULL;

	uint8 tmpType;
	PlayerId pid;

	data >> tmpType;
	data.read((uint8*)&pid, sizeof(PlayerId));

	if (tmpType == DB_PLAYER_NAME)
	{
		char name[MAX_NAME + 1] = {0};
		data.read((uint8*)name, MAX_NAME);
		info = PlayerModule::instance().findPlayerInfoByName(name);
	}
	else if (tmpType == DB_PLAYER_UID)
	{
		uint32 userId = 0;
		data >> userId;
		info = PlayerModule::instance().findPlayerInfoByUid(userId);
	}

	ByteBuffer buffer;
	buffer << (uint16)D2L_PLAYER_INFO;
	buffer.append((uint8*)&pid, sizeof(PlayerId));

	if (info == NULL)
	{
		buffer.append((uint8)DB_EMPT);
	}
	else
	{
		buffer.append((uint8)DB_SUCC);
		buffer.append((uint8*)info, sizeof(PlayerInfo));
	}

	server->sendTo(sessionId, buffer);
}

void HANDLE_L2D_CREATE_PLAYER(uint64 sessionId, IServer *server, ByteBuffer& data)
{
	PlayerId pid;
	data.read((uint8*)&pid, sizeof(PlayerId));

	PlayerInfo info;
	data.read((uint8*)&info, sizeof(PlayerInfo));

	int ret = PlayerModule::instance().createPlayerInfo(&info);

	ByteBuffer buffer;
	buffer << (uint16)D2L_CREATE_PLAYER;
	buffer.append((uint8*)&pid, sizeof(PlayerId));
	if (ret == 0)
	{
		buffer << (uint8)DB_ERR;
	}
	else
	{
		buffer << (uint8)DB_SUCC;
	}

	server->sendTo(sessionId, buffer);
}

void register_all(FuncHandle handle[])
{
	handle[L2D_PLAYER_INFO] = &HANDLE_L2D_PLAYER_INFO;
	handle[L2D_CREATE_PLAYER] = &HANDLE_L2D_CREATE_PLAYER;
}