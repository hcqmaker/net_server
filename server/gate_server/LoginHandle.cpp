#include "GateServer.hpp"
#include "rule.hpp"

NETWORK_BEGIN

GateServer::LoginHandle::LoginHandle(GateServer *s) : m_pGateServer(s) {}
GateServer::LoginHandle::~LoginHandle(){}

void GateServer::LoginHandle::onReciveClientHandle(uint64 clientId, ByteBuffer& data) 
{
	uint16 cmd;
	data >> cmd;

	if (cmd == L2C_LOGIN)
	{
		uint64 sessionId;
		data >> sessionId;

		ByteBuffer buffer;

		buffer << (uint16)L2C_LOGIN;
		buffer.append(data.rdata(), data.rsize());

		Crypto::decrypt(buffer.data(), buffer.size(), buffer.data(), buffer.size());
		m_pGateServer->m_pCServer->sendTo(sessionId, buffer);
	}
}

void GateServer::LoginHandle::onErrorClientHandle(IClient *client, const boost::system::error_code& error) 
{
	uint64 clientId = client->getId();
	NetLib::instance().destroyClient(clientId);
	sLog.outError("client error clientId:%lld error:%s \n", clientId, error.message().c_str());
}

NETWORK_END