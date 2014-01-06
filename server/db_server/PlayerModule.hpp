#ifndef __PLAYER_MODULE_HPP__
#define __PLAYER_MODULE_HPP__

#include "base.hpp"
#include "db.hpp"
#include "net.hpp"
#include "rule.hpp"

NETWORK_BEGIN
	typedef std::map<uint32,PlayerInfo*> PlayerInfoMap;
	class PlayerModule : public Singleton<PlayerModule>
	{
	private:
		PlayerModule();
		~PlayerModule();
		friend class Singleton<PlayerModule>;

		PlayerInfoMap m_PlayerInfoMap;
		sql::Connection *m_pConn;
	public:
		PlayerInfo* findPlayerInfoByName(const char* user_name);
		PlayerInfo* findPlayerInfoByUid(uint32 userId);
		PlayerInfo* findPlayerInfoById(uint64 id);

		void delPlayerInfoByUid(uint32 userId);
		void addPlayerInfo(PlayerInfo *info);

		void updatePlayerInfo(PlayerInfo *info);
		void bind(Connection *con) { m_pConn = con; }
		
	};

NETWORK_END

#endif // __PLAYER_MODULE_HPP__
