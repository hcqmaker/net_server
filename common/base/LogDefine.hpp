#ifndef __LOG_DEFINED_HPP__
#define __LOG_DEFINED_HPP__

#include "base/Define.hpp"

//////////////////////////////////////////////
#define USER_LOG_FILE
#ifdef USER_LOG_FILE

#include "Log.hpp"

#include <assert.h>
#define LIB_ASSET(x,...) do {assert(x); if (!(x)) { printf(__VA_ARGS__); printf("\n"); } } while(0)
#define ERROR_LOG(fmt, ...) do { sLog->outError(fmt, __VA_ARGS__);} while(0)
#define DEBUG_LOG(fmt, ...) do { sLog->outMessage(fmt, __VA_ARGS__);} while(0)
#define DETAIL_LOG(fmt, ...) do { sLog->outWarning(fmt, __VA_ARGS__);} while(0)

#elif !defined(_DEBUG)

#include <assert.h>
#define LIB_ASSET(x,...) do {assert(x); if (!(x)) {printf(__VA_ARGS__); printf("\n");} } while(0)
#define ERROR_LOG(fmt, ...) 0
#define DEBUG_LOG(fmt, ...) 0
#define DETAIL_LOG(fmt, ...) do{ printf("ERROR:"); printf(fmt, __VA_ARGS__); printf("\n"); } while(0)

#else

#include <assert.h>
#define LIB_ASSET(x,...) do {assert(x); if (!(x)) { printf(__VA_ARGS__); printf("\n"); } } while(0)
#define ERROR_LOG(fmt, ...) do{ printf(fmt, __VA_ARGS__); printf("\n"); } while(0)
#define DEBUG_LOG(fmt, ...) do{ printf(fmt, __VA_ARGS__); printf("\n"); } while(0)
#define DETAIL_LOG(fmt, ...) do{ printf("ERROR:"); printf(fmt, __VA_ARGS__); printf("\n"); } while(0)

#endif

#endif // __LOG_DEFINED_HPP__