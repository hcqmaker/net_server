#ifndef __BYTECONVERTER_HPP__
#define __BYTECONVERTER_HPP__

#include "base/Define.hpp"
#include <algorithm>

NETWORK_BEGIN
	namespace ByteConverter
	{
		template<size_t T>
		inline void convert(char* val)
		{
			std::swap(*val, *(val + T - 1));
			convert < T - 2 > (val + 1);
		}

		template<> inline void convert<0>(char*) {}
		template<> inline void convert<1>(char*) {}

		template<typename T>
		inline void apply(T* val)
		{
			convert<sizeof(T)>((char*)(val));
		}
	}
	
// here use big endian
template<typename T> inline void EndianConvert(T& val) 
{ 
	if (is_littlendbian())
		ByteConverter::apply<T>(&val); 
}

template<typename T> inline void EndianConvertReverse(T& val)
{
	if (is_littlendbian())
		ByteConverter::apply<T>(&val);
}


inline void EndianConvert(uint8&) { }
inline void EndianConvert(int8&)  { }
inline void EndianConvertReverse(uint8&) { }
inline void EndianConvertReverse(int8&) { }


NETWORK_END

#endif // __BYTECONVERTER_HPP__