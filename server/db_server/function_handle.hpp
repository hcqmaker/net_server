#ifndef __FUNCTION_HANDLE_HPP__
#define __FUNCTION_HANDLE_HPP__

#include "rule.hpp"
#include "net.hpp"

USE_NETWORK;
typedef void (*FuncHandle)(uint64 sessionId, IServer *server, ByteBuffer& data);

void register_all(FuncHandle handle[]);

#endif // __FUNCTION_HANDLE_HPP__