#include "Crypto.hpp"

NETWORK_BEGIN

#define KEY_DATA_LEN 5
#define KEY_DATA_MULTI 0.2

	const uint8 KEY_DATA[KEY_DATA_LEN] = {0x33,0x12,0x31,0x40,0xab}; 

	bool Crypto::encrypt(uint8* out_data, size_t out_len, const uint8* in_data, size_t in_len)
	{
		return xor(out_data, out_len, in_data, in_len);
	}

	bool Crypto::decrypt(uint8* out_data, size_t out_len, const uint8* in_data, size_t in_len)
	{
		return xor(out_data, out_len, in_data, in_len);
	}

	bool Crypto::xor(uint8* out_data, size_t out_len, const uint8* in_data, size_t in_len)
	{
		if (out_len != in_len)
			return false;

		const size_t count = (size_t)(in_len * KEY_DATA_MULTI);
		size_t index = 0;
		for(size_t i = 0; i < count; i++)
		{
			out_data[index] = in_data[index] ^ KEY_DATA[0];
			index++;
			out_data[index] = in_data[index] ^ KEY_DATA[1];
			index++;
			out_data[index] = in_data[index] ^ KEY_DATA[2];
			index++;
			out_data[index] = in_data[index] ^ KEY_DATA[3];
			index++;
			out_data[index] = in_data[index] ^ KEY_DATA[4];
			index++;
		}

		size_t t = 0;
		while(index < in_len)
		{
			out_data[index] = in_data[index] ^ KEY_DATA[t];
			index++; t++;
		}

		return true;

	}
NETWORK_END