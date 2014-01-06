#include "GateServer.hpp"

NETWORK_BEGIN

GateServer::ChatHandle::ChatHandle(GateServer *s) : m_pGateServer(s) {}
GateServer::ChatHandle::~ChatHandle(){}

void GateServer::ChatHandle::onReciveClientHandle(uint64 clientId, ByteBuffer& data) {}
void GateServer::ChatHandle::onErrorClientHandle(IClient *client, const boost::system::error_code& error) {}

NETWORK_END