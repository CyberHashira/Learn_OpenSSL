/*      This is a hobby project shared for learning purposes under the MIT License.
        Use the code freely, with no warranties or guarantees — @Cyber Hashira.

        OBJECTIVE:
        - This sample demonstrates how to encrypt plaintext using the aes-cbc-pad mechanism.
        - It generates an aes key, then uses it to encrypt the plaintext, and subsequently to decrypt it.
*/

#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>

unsigned char *aesKey = NULL;
int keySize = 16;
const EVP_CIPHER *aesCtx = NULL;
unsigned char iv[16];
int aesBlockSize = 0;
unsigned char *encrypted = NULL;
unsigned char *decrypted = NULL;
int encLen,decLen,msgLen;
const char plaintext[] = "Earth is the third planet of our Solar System.";


void generateAesKey(int keySize)
{
	aesKey = (unsigned char*)malloc(keySize);
	RAND_bytes(aesKey, keySize);
	switch(keySize)
	{
		case 16: aesCtx = EVP_aes_128_cbc();
		break;
		case 24: aesCtx = EVP_aes_192_cbc();
		break;
		case 32: aesCtx = EVP_aes_256_cbc();
		break;
	}
	printf("\n  --> AES-%d key generated.", (keySize * 8));
}


void generateIV()
{
	RAND_bytes(iv, 16);
	printf("\n  --> IV generated.");
}


void bytesToHex(const char *message, size_t msgLen)
{
	for(int ctr=0;ctr<msgLen;ctr++)
		printf("%02x", (unsigned char)message[ctr]);

	printf("\n");
}


void encryptData()
{
	int len=0;
	EVP_CIPHER_CTX *encCtx = EVP_CIPHER_CTX_new();

	EVP_EncryptInit_ex(encCtx, aesCtx, NULL, aesKey, iv);
	aesBlockSize = EVP_CIPHER_block_size(EVP_aes_128_cbc());
	len = msgLen;
	encrypted = (unsigned char*)malloc(len + aesBlockSize);

	EVP_EncryptUpdate(encCtx, encrypted, &len, (unsigned char*)plaintext, strlen(plaintext));
	encLen = len;

	EVP_EncryptFinal_ex(encCtx, encrypted + len, &len);
	encLen += len;

	EVP_CIPHER_CTX_free(encCtx);
	printf("\n  --> Plaintext encrypted.");
}


void decryptData()
{
	int len=0;
	EVP_CIPHER_CTX *decCtx = EVP_CIPHER_CTX_new();

	EVP_DecryptInit_ex(decCtx, aesCtx, NULL, aesKey, iv);
	decrypted = (unsigned char*)malloc(encLen);
	len = encLen;

	EVP_DecryptUpdate(decCtx, decrypted, &len, encrypted, encLen);
	decLen = len;

	EVP_DecryptFinal_ex(decCtx, decrypted + len, &decLen);
	decLen += len;

	EVP_CIPHER_CTX_free(decCtx);

	printf("\n  --> Encrypted data decrypted.");
}


int main()
{
	printf("\nEnter AES key size to use (16/24/32): ");
	scanf("%d", &keySize);
	if(keySize!=16 && keySize!=24 && keySize!=32)
	{
		fprintf(stderr, "\nInvalid AES key size.\n");
		exit(1);
	}
	generateAesKey(keySize);
	generateIV();

	msgLen = strlen(plaintext);

	encryptData();
	decryptData();

	printf("\n  --> Results :\n");
	printf("      + Plaintext       : %s.\n", plaintext);
	printf("      + Plaintext (hex) : "); bytesToHex(plaintext, msgLen);
	printf("      + Encrypted (hex) : "); bytesToHex((const char*)encrypted, encLen);
	printf("      + Decrypted (hex) : "); bytesToHex((const char*)decrypted, decLen);
	printf("\n\n");

	free(aesKey);
	free(encrypted);
	free(decrypted);

	return 0;
}
