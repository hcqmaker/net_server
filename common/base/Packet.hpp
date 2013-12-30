#ifndef __PACKET_HPP__
#define __PACKET_HPP__

#include "Define.hpp"
#include "ByteBuffer.hpp"

NETWORK_BEGIN

	class Packet : public ByteBuffer
	{
	public:
		Packet()
		: ByteBuffer(0)
		, m_nCmd(0) {}

		explicit Packet(uint16 cmd, size_t res = 200)
			: ByteBuffer(res)
			, m_nCmd(cmd)
		{

		}

		void Initialize(uint16 cmd, size_t newres = 200)
		{
			clear();
			_storage.reserve(newres);
			m_nCmd = cmd;
		}

		uint16 GetCmd() const { return m_nCmd; }
		void SetCmd(uint16 cmd) { m_nCmd = cmd;}

		void skipHead()
		{
			_wpos = 0;
			//append(uint32(0));	// jump length data
			append(uint16(0));	// jump length cmd
		}

		void encode()
		{
			_wpos = 0;
			//append(size());	// write length data
			append(m_nCmd);	// write length cmd
		}

		bool decode()
		{
			_rpos = 0;
			//size_t s = read<uint32>();
			m_nCmd = read<uint16>();
			return true;
		}

		const uint8* data() const {return &_storage[0]; }
		uint8* data() { return &_storage[0]; }

		size_t size() const { return _storage.size(); }

	protected:
		uint16 m_nCmd;
	};

NETWORK_END

#endif // __PACKET_HPP__