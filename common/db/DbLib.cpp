#include "DbLib.hpp"


NETWORK_BEGIN
	DbLib::DbLib()
	{
		m_pDriver = sql::mysql::get_mysql_driver_instance();
	}

	DbLib::~DbLib()
	{

	}

	Connection *DbLib::connect(const char* hostName, const char* userName, const char* password)
	{
		return m_pDriver->connect(hostName,userName, password);
	}

NETWORK_END