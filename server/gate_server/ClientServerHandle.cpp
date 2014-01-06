#include "GateServer.hpp"
#include "rule.hpp"

NETWORK_BEGIN

GateServer::ClientServerHandle::ClientServerHandle(GateServer *s) : m_pGateServer(s) {}
GateServer::ClientServerHandle::~ClientServerHandle(){}

void GateServer::ClientServerHandle::onReciveServerHandle(uint64 serverId, uint64 sessionId, ByteBuffer& data) 
{
	data.read_skip(4);
	Crypto::decrypt(data.rdata(), data.rsize(), data.rdata(), data.rsize());
	uint16 cmd = data.read<uint16>();
	if (cmd > LOGIN_BEGIN && cmd < LOGIN_END)
	{
		// cmd, sessionId, data....
		ByteBuffer buffer;
		buffer << cmd;
		buffer << sessionId;
		buffer.append(data.rdata(), data.rsize());
		m_pGateServer->m_pLogin->write(buffer);
	}
}

void GateServer::ClientServerHandle::onErrorServerHandle(IServer *server, const boost::system::error_code& error) 
{
	uint64 serverId = server->getId();
	NetLib::instance().destroyServer(serverId);
	sLog.outError("server error serverId:%ld error:%s \n", serverId, error.message().c_str());
}

NETWORK_END
