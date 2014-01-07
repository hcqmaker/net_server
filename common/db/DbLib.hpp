#ifndef __DB_LIB_HPP__
#define __DB_LIB_HPP__

#include "base/Define.hpp"
#include "base/Singleton.hpp"

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>
#include <string.h>

using namespace sql;
NETWORK_BEGIN
	class DbLib : public Singleton<DbLib>
	{
	private:

		mysql::MySQL_Driver *m_pDriver;

		DbLib();
		~DbLib();
		friend class Singleton<DbLib>;
	public:
		Connection *connect(const char* hostName, const char* userName, const char* password);

	};

NETWORK_END


#endif // __DB_LIB_HPP__