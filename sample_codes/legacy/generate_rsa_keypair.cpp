/*
	- Thanks for reading DISCLAIMER.txt
	This sample demonstrates how to generate RSA keypair using OpenSSL API.
	This method of generating RSA keypair is now deprecated by OpenSSL.
*/

#include <iostream>
#include <openssl/rsa.h>
#include <openssl/bio.h>

using namespace std;

int main()
{
	int keySize = 0;
	BIO *bioMem;
	unsigned char *buff = NULL;

	cout << "RSA keysize to generate : ";
	cin >> keySize;
	buff = new unsigned char[keySize];

	RSA *rsa;
	rsa = RSA_generate_key(keySize, RSA_F4, NULL, NULL);
	bioMem = BIO_new(BIO_s_mem());

	if(RSA_check_key(rsa)!=1)
	{
		cout << "problem generating key.";
		exit(1);
	}

	RSA_print(bioMem, rsa, 0);
	while(BIO_read(bioMem, buff, keySize)>0)
	{
		cout << buff;
	}
	BIO_free(bioMem);
	cout << endl;
	return 0;
}
