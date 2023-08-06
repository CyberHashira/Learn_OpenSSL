/*
	- Thanks for reading DISCLAIMER.txt
	This sample demonstrates how to calculate MD5 message digest.
	This method of calculating hash is deprecated since OpenSSL 3.x and it is now a part of OpenSSL legacy API.
*/


#include <iostream>
#include <openssl/md5.h>

using namespace std;

int main()
{
	MD5_CTX md5;
	char text[] = "Hello World";
	unsigned char DIGEST[MD5_DIGEST_LENGTH];

	MD5_Init(&md5);
	MD5_Update(&md5, text, sizeof(text)-1);
	MD5_Final(DIGEST, &md5);


	for(int ctr=0; ctr<MD5_DIGEST_LENGTH; ctr++)
	{
		printf("%02x:", DIGEST[ctr]);
	}
	cout << endl;
	return 0;
}
