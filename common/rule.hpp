#ifndef __RULE_HPP__
#define __RULE_HPP__

#include "base.hpp"

#pragma pack(push, 1)

// all game agreement

enum NET_RULE
{
	// Login
	LOGIN_BEGIN = 201,
	C2L_LOGIN,
	L2C_LOGIN,
	C2L_CREATE,
	L2C_CREATE,
	LOGIN_END,


	// db
	DB_BEGIN = 400,
	L2D_PLAYER_INFO,
	D2L_PLAYER_INFO,
	L2D_CREATE_PLAYER,
	D2L_CREATE_PLAYER,

	S2D_PLAYER_INFO,	// get player info 0: name 1: user_id
	D2S_PLAYER_INFO,		// response player info 
	DB_END,

	RULE_NUM
};


#define MAX_NAME 16
#define MAX_PWD 8

enum DB_PLAYER_TYPE
{
	DB_PLAYER_NAME = 0,
	DB_PLAYER_UID
};

enum DB_RET
{
	DB_ERR = 0,
	DB_SUCC,
	DB_EMPT,
};


struct PlayerId
{
	uint64 csessionId;	// client session from gate server
	uint64 lsessionId;	// login session
	uint64 serverId;

	PlayerId(uint64 csId = 0, uint64 lsId = 0, uint64 sId = 0)
		:csessionId(csId), lsessionId(lsId), serverId(sId) {}
	bool operator==(const PlayerId& s) const
	{
		return this->csessionId == s.csessionId && this->lsessionId == s.lsessionId && this->serverId == s.serverId;
	}
	bool operator<(const PlayerId& s) const
	{
		return this->csessionId < s.csessionId && this->lsessionId < s.lsessionId && this->serverId < s.serverId;
	}
	bool operator!=(const PlayerId& s) const
	{
		return this->csessionId == s.csessionId && this->lsessionId == s.lsessionId && this->serverId == s.serverId;
	}
};

struct PlayerLoginInfo
{
	char user_name[MAX_NAME];
	char user_pwd[MAX_PWD];
	PlayerLoginInfo() { memset(this, 0, sizeof(PlayerLoginInfo)); }
};

struct PlayerInfo
{
	uint32 user_id;
	char user_name[MAX_NAME];
	char user_pwd[MAX_PWD];
	uint16 level;
	uint8 gender;
	uint32 role_id;
	uint32 scene_id;
	uint32 x;
	uint32 y;
	PlayerInfo() { memset(this, 0, sizeof(PlayerInfo));}
};

#pragma pack(pop)
#endif // __RULE_HPP__