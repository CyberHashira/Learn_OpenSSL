/*
	- Thanks for reading DISCLAIMER.txt
	This sample demonstrates the usage of OpenSSL EVP to calculate message digest.
	OpenSSL EVP is recommended over the old legacy API.
*/

#include <iostream>
#include <openssl/evp.h>

using namespace std;

int main()
{
	const EVP_MD *md; // Stores implementation for a message digest
	EVP_MD_CTX *mdCtx; // Stores the context for message digest

	char text[] = "Earth is the third planet of our Solar System.";
	unsigned char DIGEST[EVP_MAX_MD_SIZE];
	unsigned int hashLen;
	char algorithm[10];

	cout << "Algorithm to use (md5, sha1, sha256, sha384, sha3-256, ...) : ";
	cin >> algorithm;

	OpenSSL_add_all_digests();
	md = EVP_get_digestbyname(algorithm);

	if(!md)
	{
		cout << "Requested message digest algorithm was not found." << endl;
		exit(1);
	}

	mdCtx = EVP_MD_CTX_new(); // creates a new message digest context.
	EVP_DigestInit_ex(mdCtx, md, NULL); // initializes message digest context.
	EVP_DigestUpdate(mdCtx, text, sizeof(text)-1); // updates message digest context.
	EVP_DigestFinal(mdCtx, DIGEST, &hashLen); // finalizes message digest context.

	for(int ctr=0; ctr<hashLen; ctr++)
	{
		printf("%02x", DIGEST[ctr]);
	}
	cout << endl;
	return 0;
}
