#ifndef __RULE_HPP__
#define __RULE_HPP__

#pragma pack(push, 1)

// all game agreement

enum NET_RULE
{
	// Login
	LOGIN_BEGIN = 201,
	C2L_LOGIN,
	L2C_LOGIN,
	LOGIN_END,


	// db
	DB_BEGIN = 400,
	L2D_PLAYER_INFO,
	D2L_PLAYER_INFO,

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
};

struct PlayerLoginInfo
{
	char user_name[MAX_NAME];
	char user_pwd[MAX_PWD];
	PlayerLoginInfo() { memset(this, 0, sizeof(PlayerLoginInfo)); }
};

struct PlayerInfo
{
	uint64 id;
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