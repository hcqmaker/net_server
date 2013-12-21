#include <iostream>
#include "Crypto.hpp"

using namespace std;

int main(int argc, char** argv)
{
	USE_NETWORK;
	const size_t size = 37;
	uint8 in_data[] = "aabbccddeeffzho293890fuoiwugkljvxlkcj";
	uint8 out_data[size + 1] = {0};
	
	Crypto::encrypt(out_data, size, in_data, size);
	printf("encrypt: %s\n", out_data);
	uint8 out_data2[size + 1] = {0};
	Crypto::decrypt(out_data2, size, out_data, size);
	printf("decrypt: %s\n", out_data2);

	system("pause");
	return 0;
}