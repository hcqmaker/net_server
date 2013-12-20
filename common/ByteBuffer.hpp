#ifndef __BYTEBUFFER_HPP__
#define __BYTEBUFFER_HPP__

#include "Define.hpp"
#include "LogDefine.hpp"
#include "ByteConverter.hpp"

#include <vector>

NETWORK_BEGIN

	class ByteBufferException
	{
	public:
		ByteBufferException(bool add, size_t pos, size_t esize, size_t size)
			: _add(add), _pos(pos), _esize(esize), _size(size)
		{
			PrintPosError();
		}
		void PrintPosError() const
		{

		}
	private:
		bool _add;
		size_t _pos;
		size_t _esize;
		size_t _size;
	};


	template<typename T>
	struct unused 
	{
		unused(){}
	};

	class ByteBuffer
	{
	public:
		const static size_t DEFAULT_SIZE = 4096;
		explicit ByteBuffer(): _rpos(0), _wpos(0) { _storage.reserve(DEFAULT_SIZE); }
		ByteBuffer(size_t res): _rpos(0), _wpos(0) { _storage.reserve(res); }
		ByteBuffer(const ByteBuffer& buf):_rpos(buf._rpos), _wpos(buf._wpos),_storage(buf._storage) {}

		void clear() { _storage.clear(); _rpos = _wpos = 0; }

		template<typename T> void append(T value)
		{
			EndianConvert(value);
			append((uint8*)&value, sizeof(value));
		}

		template<typename T> void put(size_t pos, T value)
		{
			EndianConvert(value);
			put(pos, (uint8*)&value, sizeof(value));
		}

		ByteBuffer& operator<<(uint8 value) { append<uint8>(value); return *this; }
		ByteBuffer& operator<<(uint16 value) { append<uint16>(value); return *this; }
		ByteBuffer& operator<<(uint32 value) { append<uint32>(value); return *this; }
		ByteBuffer& operator<<(uint64 value) { append<uint64>(value); return *this; }

		ByteBuffer& operator<<(int8 value) { append<int8>(value); return *this; }
		ByteBuffer& operator<<(int16 value) { append<int16>(value); return *this; }
		ByteBuffer& operator<<(int32 value) { append<int32>(value); return *this; }
		ByteBuffer& operator<<(int64 value) { append<int64>(value); return *this; }

		ByteBuffer& operator<<(float value) { append<float>(value); return *this; }
		ByteBuffer& operator<<(double value) { append<double>(value); return *this; }
		ByteBuffer& operator<<(const std::string& value) { append((uint8 const*)value.c_str(), value.length()); append((uint8)0); return *this; }
		ByteBuffer& operator<<(const char* value) { append((uint8 const*)value, value ? strlen(value) : 0); append((uint8)0); return *this; }

		ByteBuffer& operator>>(bool& value) { value = read<char>() > 0 ? true : false; return *this; }
		ByteBuffer& operator>>(uint8& value) { value = read<uint8>(); return *this; }
		ByteBuffer& operator>>(uint16& value) { value = read<uint16>(); return *this; }
		ByteBuffer& operator>>(uint32& value) { value = read<uint32>(); return *this; }
		ByteBuffer& operator>>(uint64& value) { value = read<uint64>(); return *this; }

		ByteBuffer& operator>>(int8& value) { value = read<int8>(); return *this; }
		ByteBuffer& operator>>(int16& value) { value = read<int16>(); return *this; }
		ByteBuffer& operator>>(int32& value) { value = read<int32>(); return *this; }
		ByteBuffer& operator>>(int64& value) { value = read<int64>(); return *this; }

		ByteBuffer& operator>>(float& value) { value = read<float>(); return *this; }
		ByteBuffer& operator>>(double& value) { value = read<double>(); return *this; }
		ByteBuffer& operator>>(std::string& value) 
		{
			value.clear();
			while (rpos() < size())
			{
				char c = read<char>();
				if (c == 0)
					break;
				value += c;
			}
			return *this;
		}
		template<typename T>
		ByteBuffer& operator>>(unused<T> const&) { read_skip<T>(); return *this; }
		uint8 operator[](size_t pos) const { return read<uint8>(pos);}
		size_t rpos() const { return _rpos; }
		size_t rpos(size_t rpos) { _rpos = rpos; return _rpos; }
		size_t wpos() const { return _wpos; }
		size_t wpos(size_t wpos) { _wpos = wpos; return _wpos; }

		template<typename T>
		void read_skip() { read_skip(sizeof(T)); }
		void read_skip(size_t skip)
		{
			if (_rpos + skip > size())
				throw ByteBufferException(false, _rpos, skip, size());
			_rpos += skip;
		}

		template <typename T> T read()
		{
			T r = read<T>(_rpos);
			_rpos += sizeof(T);
			return r;
		}

		template<typename T>
		T read(size_t pos) const 
		{
			if (pos + sizeof(T) > size())
				throw ByteBufferException(false, pos, sizeof(T), size());
			T val = *((T const*)&_storage[pos]);
			EndianConvert(val);
			return val;
		}

		void read(uint8* dest, size_t len)
		{
			if (_rpos + len > size())
				throw ByteBufferException(false, _rpos, len, size());
			std::memcpy(dest, &_storage[_rpos], len);
			_rpos += len;
		}

		uint64 readPackGUID()
		{
			uint64 guid = 0;
			uint8 guidmark = 0;
			(*this) >> guidmark;
			for (int i = 0; i < 8; ++i)
			{
				if (guidmark &(uint8(1) << 1))
				{
					uint8 bit;
					(*this) >> bit;
					guid |= (uint64(bit) << (i * 8));
				}
			}
			return guid;
		}

		const uint8* contents() const { return &_storage[0]; }
		const uint8* data() const { return &_storage[0]; }
		std::vector<uint8>& bytes() { return _storage; }
		size_t size() const { return _storage.size(); }
		bool empty() const { return _storage.empty(); }
		void resize(size_t newsize) { _storage.resize(newsize); _rpos = 0; _wpos = size(); }
		void reserve(size_t ressize) { if (ressize > size()) _storage.reserve(ressize); }
		
		void append(const std::string& str) { append((uint8 const*)str.c_str(), str.size() + 1);}
		void append(const char* src, size_t cnt) { append((const uint8*)src, cnt); }

		template<class T> 
		void append(const T* src, size_t cnt) { append((const uint8*)src, cnt * sizeof(T)); }
		void append(const uint8* src, size_t cnt)
		{
			if (!cnt)
				return;
			LIB_ASSET(size() < 10000000, "out of max size");
			if (_storage.size() < _wpos + cnt)
				_storage.resize(_wpos + cnt);
			std::memcpy(&_storage[_wpos], src, cnt);
			_wpos += cnt;
		}
		void append(const ByteBuffer& buffer)
		{
			if (buffer.wpos())
				append(buffer.contents(), buffer.wpos());
		}

		void appendPackXYZ(float x, float y, float z)
		{
			uint32  packed = 0;
			packed |= ((int) (x / 0.25f) & 0x7ff);
			packed |= ((int) (y / 0.25f) & 0x7ff) << 11;
			packed |= ((int) (z / 0.25f) & 0x3ff) << 22;
			*this << packed;
		}

	protected:
		size_t _rpos, _wpos;
		std::vector<uint8> _storage;
	};


NETWORK_END



#endif // __BYTEBUFFER_HPP__ 