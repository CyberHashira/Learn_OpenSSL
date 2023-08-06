/*
	- Thanks for reading DISCLAIMER.txt
	This sample code has three objectives :-
	1. Generate RSA based keypair.
	2. Encrypt a text using RSA-OAEP padding.
	3. Decrypt the encrypted string.
*/


#include <iostream>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/err.h>

using namespace std;

EVP_PKEY *rsaKey = NULL;

const int keySize = 2048; // Change this value to use another keysize.
const char message[] = "Earth is the third planet of our Solar System."; // Text to be encrypted.
unsigned char *encrypted = NULL; size_t encLen; // Stores Ciphertext.
unsigned char *decrypted = NULL; size_t decLen; // Stores decrypted text.


// Generate RSA Keypair
void generateKeyPair()
{
	EVP_PKEY_CTX *rsaCtx = NULL;
	rsaCtx =  EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
	EVP_PKEY_keygen_init(rsaCtx);
	EVP_PKEY_CTX_set_rsa_keygen_bits(rsaCtx, keySize);
	if(EVP_PKEY_keygen(rsaCtx, &rsaKey)!=1)
	{
		cout << "Failed to generate RSA keypair." << ERR_error_string(ERR_get_error(), NULL) << endl;
		exit(1);
	}
	EVP_PKEY_CTX_free(rsaCtx);
}


// Encrypts plaintext using RSA-OAEP padding.
void encryptData()
{
	EVP_PKEY_CTX *encCtx = EVP_PKEY_CTX_new(rsaKey, NULL); // Creating a new context for encryption.
	EVP_PKEY_encrypt_init(encCtx); // Initializing encryption context.
	EVP_PKEY_CTX_set_rsa_padding(encCtx, RSA_PKCS1_OAEP_PADDING); // Setting padding mode.
	encLen = keySize/8; // Determining the expected output length.
	encrypted = new unsigned char[encLen]; // allocated memory to store ciphertext.
	EVP_PKEY_encrypt(encCtx, encrypted, &encLen, (const unsigned char*)message, sizeof(message)-1); // Encrypts plaintext.
	if(ERR_get_error()!=0) // Error handing.
	{
		cout << "Encryption failed. Reason : " << ERR_error_string(ERR_get_error(), NULL) << endl;
	}
	EVP_PKEY_CTX_free(encCtx); // Free context used for encryption.
}


// Decrypts encrypted text using RSA-OAEP padding.
void decryptData()
{
	EVP_PKEY_CTX *decCtx = EVP_PKEY_CTX_new(rsaKey, NULL); // Creating a new context for decryption.
	EVP_PKEY_decrypt_init(decCtx); // Initializing decryption context.
	EVP_PKEY_CTX_set_rsa_padding(decCtx, RSA_PKCS1_OAEP_PADDING); // Setting padding mode.
	EVP_PKEY_decrypt(decCtx, NULL, &decLen, encrypted, encLen); // Mock decrypt to predict the output length.
	decrypted = new unsigned char[decLen]; // Allocated memory to output.
	EVP_PKEY_decrypt(decCtx, decrypted, &decLen, encrypted, encLen); // Decrypt data.
	
	if(ERR_get_error()!=0) // Error handling.
	{
		cout << "Decryption failed. Reason : " << ERR_error_string(ERR_get_error(), NULL) << endl;
	}
	EVP_PKEY_CTX_free(decCtx); // Freeing decrypt context.
}

// Display byte array as hex.
void encodeHex(unsigned char *data, int len)
{
	for(int ctr=0; ctr<len; ctr++)
	{
		printf("%02x", data[ctr]);
	}
	cout << endl;
}


int main()
{
	generateKeyPair();
	cout << "RSA keypair generated..." << endl;
	encryptData();
	cout << "Message encrypted..." << endl;
	decryptData();
	cout << "Message decrypted..." << endl;
	cout << endl << "Results - " << endl;
	cout << "Plain Text	\t : " << message << endl;
	cout << "Plain Text as hex \t : "; encodeHex((unsigned char*)message, sizeof(message)-1);
	cout << "Ciphertext as hex \t : "; encodeHex(encrypted, encLen);
	cout << "Decrypted Text    \t : " << decrypted << endl;
	cout << "Decrypted text as hex \t : "; encodeHex(decrypted, decLen);
	encrypted = NULL;
	decrypted = NULL;
	return 0;
}