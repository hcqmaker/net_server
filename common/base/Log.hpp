#ifndef __LOG_HPP__
#define __LOG_HPP__


#include "base/Define.hpp"
#include "base/Singleton.hpp"

#include <cstdio>
#include <string>

NETWORK_BEGIN
	class Log : public Singleton<Log>
	{
	private:
		Log();
		~Log();
		friend class Singleton<Log>;

		FILE *m_pFileMessage;
		FILE *m_pFileWarning;
		FILE *m_pFileError;

		void outString(FILE *file, const char* data, size_t size);
	public:
		static std::string currentSystemTime();

		void outMessage(const char* fmt, ...);
		void outWarning(const char* fmt, ...);
		void outError(const char* fmt, ...);
	};

	
#define sLog Log::instance()
NETWORK_END

#endif // __LOG_HPP__