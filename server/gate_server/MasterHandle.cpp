#include "GateServer.hpp"

NETWORK_BEGIN
	
	GateServer::MasterHandle::MasterHandle(GateServer *s) : m_pGateServer(s) {}
	GateServer::MasterHandle::~MasterHandle(){}

	void GateServer::MasterHandle::onReciveClientHandle(uint64 clientId, ByteBuffer& data) {}
	void GateServer::MasterHandle::onErrorClientHandle(IClient *client, const boost::system::error_code& error) {}

NETWORK_END