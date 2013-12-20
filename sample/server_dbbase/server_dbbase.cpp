#include <iostream>
#include "DbLib.hpp"
#include <string.h>
using namespace std;

/************************************************************************/
/* 
DB use mysql 
need MySQL Connector/C++ 
download from mysql web

===== windows =======
include/
	mysql_driver.h
	mysql_connection.h
	cppconn/....
lib/
	mysqlcppconn.dll
	mysqlcppconn.lib
	mysqlcppconn-static.lib
*/
/************************************************************************/


int main(int argc, char** argv)
{
	USE_NETWORK;
	using namespace sql;
	Statement *state;
	ResultSet *result;

	Connection *con = DbLib::instance().connect("tcp://127.0.0.1:3306", "root", "");
	state = con->createStatement();
	state->execute("use net_db");
	result = state->executeQuery("select * from g_user");
	//printf(" id user_id user_name level profession\n");
	printf("%7s%10s%20s%7s%10s\n", "id", "user_id", "user_name", "level", "role_id");
	while(result->next())
	{
		int64_t id		= result->getInt64("id");
		int user_id		= result->getInt("user_id");
		string user_name = result->getString("user_name");
		int level		= result->getInt("level");
		int profession	= result->getInt("role_id");

		printf(" %7d ", id);
		printf(" %10d ", user_id);
		printf(" %20s ", user_name.c_str());
		printf(" %7d ", level);
		printf(" %10d \n", profession);
	}

	delete state;
	delete con;

	system("pause");

	return 0;
}