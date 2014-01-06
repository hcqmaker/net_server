#include "GateServer.hpp"

NETWORK_BEGIN

GateServer::GameServerHandle::GameServerHandle(GateServer *s) : m_pGateServer(s) {}
GateServer::GameServerHandle::~GameServerHandle(){}

void GateServer::GameServerHandle::onReciveServerHandle(uint64 serverId, uint64 sessionId, ByteBuffer& data) {}
void GateServer::GameServerHandle::onErrorServerHandle(IServer *server, const boost::system::error_code& error) {}

NETWORK_END
