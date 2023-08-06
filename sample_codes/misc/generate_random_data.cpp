/*
	- Thanks for reading DISCLAIMER.txt
	This sample demonstrates how to generate 32 bytes of random data using OpenSSL API.
*/

#include <iostream>
#include <openssl/rand.h>

using namespace std;

int main()
{
	unsigned char random_data[32];
	RAND_bytes(random_data, 32);

	for(int ctr=0; ctr<32; ctr++)
	{
		printf("%02x",random_data[ctr]);
	}
	cout << endl;
	return 0;
}
