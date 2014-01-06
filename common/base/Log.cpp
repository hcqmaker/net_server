#include "Log.hpp"


#include <cstdarg>
#include <cstdio>
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>

#ifdef _MSC_VER
#include <Windows.h>
#endif 

#define BOOST_DATE_TIME_SOURCE 
#define MAX_QUERY_LEN 32*1024
#define RETURN_IS_FAIL(v) {if (!v) return;}

#if PLATFORM == PLATFORM_WINDOWS

#define CACHE_VA_LIST(D, F) \
	va_list ap; \
	char D[MAX_QUERY_LEN] = {0}; \
	va_start(ap, F);\
	vsnprintf_s(D, MAX_QUERY_LEN, MAX_QUERY_LEN, F, ap);\
	va_end(ap)


#else

#define CACHE_VA_LIST(D, F) \
	va_list ap; \
	va_start(ap, F); \
	char D[MAX_QUERY_LEN] = {0}; \
	vsnprintf(D, MAX_QUERY_LEN, F, ap); \
	va_end(ap)
#endif 

NETWORK_BEGIN

	Log::Log()
	{
		m_pFileMessage = fopen("info.log", "a");
		m_pFileWarning = fopen("warn.log", "a");
		m_pFileError = fopen("error.log", "a");
	}

	//-------------------------------------------------------------------------
	//
	Log::~Log()
	{
		if (m_pFileMessage)
			fclose(m_pFileMessage);
		if (m_pFileWarning)
			fclose(m_pFileWarning);
		if (m_pFileError)
			fclose(m_pFileError);
		m_pFileMessage = m_pFileWarning = m_pFileError = NULL;
	}

	//-------------------------------------------------------------------------
	//
	void Log::outString(FILE *file, const char* data, size_t size)
	{
		std::string strTime = currentSystemTime();
		strTime.append(" ");
#if _MSC_VER
		WCHAR str[MAX_QUERY_LEN + 1] = {0};
		
		MultiByteToWideChar(CP_ACP, 0, strTime.c_str(), strTime.size() + 1, str, sizeof(str)/sizeof(str[0]));
		OutputDebugString(str);
		MultiByteToWideChar(CP_ACP, 0, data, size + 1, str, sizeof(str)/sizeof(str[0]));
		str[size] = '\n';
		str[size + 1] = '\0';
		OutputDebugString(str);
#endif 
		printf("%s %s\n", strTime.c_str(), data);

		fwrite(strTime.c_str(), strTime.size(), 1, file);
		fwrite(data, size, 1, file);
	}

	//-------------------------------------------------------------------------
	//
	std::string Log::currentSystemTime()
	{
		std::string strTime = boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time()); 
		int pos = strTime.find('T');
		strTime.replace(pos,1," ");  
		strTime.replace(pos + 3,0,":");  
		strTime.replace(pos + 6,0,":");  
		strTime.replace(6, 0, "-");
		strTime.replace(4, 0, "-");
		return strTime;
	}

	//-------------------------------------------------------------------------
	//
	void Log::outMessage(const char* fmt, ...)
	{
		RETURN_IS_FAIL(m_pFileMessage);
		CACHE_VA_LIST(data, fmt);
		outString(m_pFileMessage, data, strlen(data));
	}

	//-------------------------------------------------------------------------
	//
	void Log::outWarning(const char* fmt, ...)
	{
		RETURN_IS_FAIL(m_pFileWarning);
		CACHE_VA_LIST(data, fmt);
		outString(m_pFileWarning, data, strlen(data));
	}

	//-------------------------------------------------------------------------
	//
	void Log::outError(const char* fmt, ...)
	{
		RETURN_IS_FAIL(m_pFileError);
		CACHE_VA_LIST(data, fmt);
		outString(m_pFileError, data, strlen(data));
	}

NETWORK_END