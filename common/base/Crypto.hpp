#ifndef __CRYPTO_HPP__
#define __CRYPTO_HPP__

#include "base/Define.hpp"

NETWORK_BEGIN
	class Crypto 
	{
	public:
		static bool encrypt(uint8* out_data, size_t out_len, const uint8* in_data, size_t in_len);
		static bool decrypt(uint8* out_data, size_t out_len, const uint8* in_data, size_t in_len);

		static bool xor(uint8* out_data, size_t out_len, const uint8* in_data, size_t in_len);
	};

NETWORK_END

#endif // __CRYPTO_HPP__