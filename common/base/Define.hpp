#ifndef __DEFINE_HPP__
#define __DEFINE_HPP__

// some define different platform
#define PLATFORM_WINDOWS	0
#define PLATFORM_UNIX		1
#define PLATFORM_APPLE		2

#if defined(_WIN64)
#	define PLATFORM PLATFORM_WINDOWS
#	define WIN_X64
#elif defined(__WIN32__) || defined(WIN32)
#	define PLATFORM PLATFORM_WINDOWS
#	define WIN_X86
#elif defined(__APPLE_CC__)
#	define PLATFORM PLATFORM_APPLE
#else
#	define PLATFORM PLATFORM_UNIX
#endif


// different compiler
#define COMPILER_MICROSOFT	0
#define COMPILER_GUN		1

#ifdef _MSC_VER
#	define COMPILER COMPILER_MICROSOFT
#else
#	define COMPILER COMPILER_GUN
#endif

//////////////////////////////////////////////
#if PLATFORM == PLATFORM_WINDOWS
typedef __int64            int64;
typedef __int32            int32;
typedef __int16            int16;
typedef __int8             int8;
typedef unsigned __int64   uint64;
typedef unsigned __int32   uint32;
typedef unsigned __int16   uint16;
typedef unsigned __int8    uint8;
#else
#include <stdint.h>
#ifndef uint64_t
#ifdef __linux__
#include <linux/types.h>
#endif
#endif
typedef int64_t            int64;
typedef int32_t            int32;
typedef int16_t            int16;
typedef int8_t             int8;
typedef uint64_t           uint64;
typedef uint32_t           uint32;
typedef uint16_t           uint16;
typedef uint8_t            uint8;
#endif

#if PLATFORM != PLATFORM_WINDOWS

#define BIGENDIAN 0
#define LITTLEENDIAN 1

#endif



#define NETWORK_BEGIN namespace network {
#define NETWORK_END }
#define USE_NETWORK using namespace network


const int endian = 1;
#define is_bigendian() ( (*(char*) &endian) == 0 )
#define is_littlendbian() ( (*(char*) &endian) == 1 )

// sizeof message char
#define HEAD_LENGTH 4
// sizeof message cmd char
#define CMD_LENGTH 2

#define MAX_PACKET 4096

#endif // __DEFINE_HPP__