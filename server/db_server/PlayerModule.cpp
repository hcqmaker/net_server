#include "PlayerModule.hpp"

NETWORK_BEGIN

	// find user info
const std::string PLAYER_SELECT_PRIX = "select id,user_id,user_name,user_pwd,level,gender,role_id,scene_id,x,y from g_user where ";

	PlayerInfo * db_findPlayerInfo(Connection *conn, const std::string &sql)
	{
		PlayerInfo *info = NULL;
		Statement *stmt = conn->createStatement();
		ResultSet *res = stmt->executeQuery(sql.c_str());

		if (res->next())
		{
			info = new PlayerInfo;

			info->id = res->getInt64("id");
			info->user_id = res->getUInt("user_id");
			std::string user_name = res->getString("user_name");
			size_t size = user_name.size() < MAX_NAME ? user_name.size() : MAX_NAME;
			memcpy(info->user_name, user_name.c_str(), size);
			info->level = res->getInt("level");
			info->gender = res->getInt("gender");
			info->role_id = res->getInt("role_id");
			info->scene_id = res->getInt("scene_id");
			info->x = res->getInt("x");
			info->y = res->getInt("y");
			std::string user_pwd = res->getString("user_pwd");
			size = user_pwd.size() < MAX_PWD ? user_pwd.size() : MAX_PWD;
			memcpy(info->user_pwd, user_pwd.c_str(), size);
		}

		delete stmt;
		delete res;
		return info;
	}

	int db_updatePlayerInfo(Connection *conn, PlayerInfo *info)
	{
		std::string sql = "update g_user set ";
		sql.append("user_name='");
		sql.append(info->user_name);
		sql.append("',user_pwd='");
		sql.append("',level=");
		sql.append(info->level + "");
		sql.append(",gender=");
		sql.append(info->gender + "");
		sql.append(",role_id=");
		sql.append(info->role_id + "");
		sql.append(",scene_id=");
		sql.append(info->scene_id + "");
		sql.append(",x=");
		sql.append(info->x + "");
		sql.append(",y=");
		sql.append(info->y + "");
		sql.append(" where user_id=");
		sql.append(info->user_id + "");

		Statement *stmt = conn->createStatement();
		int ret = stmt->executeUpdate(sql.c_str());
		delete stmt;
		return ret;
	}


	PlayerModule::PlayerModule(): m_pConn(NULL) {}
	PlayerModule::~PlayerModule() {}

	PlayerInfo* PlayerModule::findPlayerInfoByName(const char* user_name)
	{
		PlayerInfo *info = NULL;
		PlayerInfoMap::iterator i = m_PlayerInfoMap.begin();
		for (; i != m_PlayerInfoMap.end(); ++i)
		{
			if (memcmp(i->second->user_name, user_name, MAX_NAME) == 0)
			{
				info = i->second;
				break;
			}
		}

		if (info == NULL)
		{
			std::string sql = PLAYER_SELECT_PRIX + "user_name=";
			sql.append("'");
			sql.append(user_name);
			sql.append("'");

			info = db_findPlayerInfo(m_pConn, sql);
			if (info)
				m_PlayerInfoMap.insert(PlayerInfoMap::value_type(info->user_id, info));
			
		}

		return info;
	}

	PlayerInfo* PlayerModule::findPlayerInfoByUid(uint32 userId)
	{
		PlayerInfo *info = NULL;
		PlayerInfoMap::iterator i = m_PlayerInfoMap.find(userId);
		if (i == m_PlayerInfoMap.end())
		{
			std::string sql = PLAYER_SELECT_PRIX + " user_id=";
			sql.append("'");
			sql.append(userId + "");
			sql.append(",");

			info = db_findPlayerInfo(m_pConn, sql);
			if (info)
				m_PlayerInfoMap.insert(PlayerInfoMap::value_type(info->user_id, info));
		}
		else
		{
			info = i->second;
		}
		return info;
	}

	PlayerInfo* PlayerModule::findPlayerInfoById(uint64 id)
	{
		PlayerInfo *info = NULL;
		PlayerInfoMap::iterator i = m_PlayerInfoMap.begin();
		for (; i != m_PlayerInfoMap.end(); ++i)
		{
			if (i->second->id == id)
			{
				info = i->second;
				break;
			}
		}

		if (info == NULL)
		{
			std::string sql = PLAYER_SELECT_PRIX + "id=";
			sql.append("'");
			sql.append(id + "");
			sql.append(",");

			info = db_findPlayerInfo(m_pConn, sql);
			if (info)
				m_PlayerInfoMap.insert(PlayerInfoMap::value_type(info->user_id, info));

		}

		return info;
	}

	void PlayerModule::delPlayerInfoByUid(uint32 userId)
	{
		PlayerInfo *info = NULL;
		PlayerInfoMap::iterator i = m_PlayerInfoMap.find(userId);
		if (i == m_PlayerInfoMap.end())
			return;

		m_PlayerInfoMap.erase(i);
	}

	void PlayerModule::addPlayerInfo(PlayerInfo *info)
	{
		PlayerInfoMap::iterator i = m_PlayerInfoMap.find(info->user_id);
		if (i == m_PlayerInfoMap.end())
		{
			m_PlayerInfoMap.insert(PlayerInfoMap::value_type(info->user_id, info));
			return;
		}
		
		delete i->second;
		m_PlayerInfoMap[info->user_id] = info;
	}

	void PlayerModule::updatePlayerInfo(PlayerInfo *info)
	{
		db_updatePlayerInfo(m_pConn, info);
	}

NETWORK_END