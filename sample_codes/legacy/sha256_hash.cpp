/*
	- Thanks for reading DISCLAIMER.txt
    This sample demonstrates how to calculate SHA-256 message digest.
    This method of calculating hash is deprecated since OpenSSL 3.x and it is now a part of OpenSSL legacy API.
*/

#include <iostream>
#include <openssl/sha.h>

using namespace std;

int main()
{
	char text[] = "Hello World";
	unsigned char DIGEST[SHA256_DIGEST_LENGTH];

	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, text, sizeof(text)-1);
	SHA256_Final(DIGEST, &sha256);

	for(int ctr = 0; ctr< SHA256_DIGEST_LENGTH; ctr++)
	{
		printf("%02x", DIGEST[ctr]);
	}
	cout << endl;
	return 0;
}
