#ifndef __FUNCTION_HANDLE_HPP__
#define __FUNCTION_HANDLE_HPP__


#include "rule.hpp"
#include "base.hpp"
#include "net.hpp"

USE_NETWORK;

class chat_client;
typedef boost::function<void(NetClient *c)> HandleSend;
typedef boost::function<void(ByteBuffer &data)> HandleReceive;

typedef std::map<std::string, HandleSend> SHandleMap;
typedef std::map<uint16, HandleReceive> RHandleMap;

class HandleManager
{
public:
	HandleManager(){}
	~HandleManager(){}

	void registSend(const std::string& m, const HandleSend& mh)
	{
		m_hSendMap[m] = mh;
	}

	void registerReceive(uint16 cmd, const HandleReceive& ch)
	{
		m_hReceiveMap[cmd] = ch;
	}

	void onSend(const std::string& cmd, NetClient *c)
	{
		SHandleMap::iterator i = m_hSendMap.find(cmd);
		if (i == m_hSendMap.end())
		{
			sLog.outWarning("[onSend] warning can't find cmd:%s", cmd.c_str());
			return;
		}

		i->second(c);
	}

	void onReceive(ByteBuffer &data)
	{
		uint16 cmd;
		data >> cmd;
		
		RHandleMap::iterator i = m_hReceiveMap.find(cmd);
		if (i == m_hReceiveMap.end())
		{
			sLog.outWarning("[onReceive] warning can't find cmd:%d", cmd);
			return;
		}

		i->second(data);
	}

protected:
	SHandleMap m_hSendMap;
	RHandleMap m_hReceiveMap;
};


void command_send(NetClient *c,  ByteBuffer& data)
{
	uint16 cmd;
	data >> cmd;

	Crypto::encrypt(data.data(), data.size(), data.data(), data.size());

	ByteBuffer buffer;
	buffer.append((uint32)data.size());
	buffer.append(data.data(), data.size());

	c->write(buffer);
}


#endif // __FUNCTION_HANDLE_HPP__