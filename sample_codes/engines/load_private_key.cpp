/*
	- Thanks for reading DISCLAIMER.txt
	This code demonstrates how to load engines using OpenSSL API.
	This code was tested and works using GemEngine for Thales Luna HSMs.
*/


#include <iostream>
#include <openssl/engine.h>
#include <openssl/evp.h>

using namespace std;

ENGINE *engine = NULL;
char *engine_name = NULL;
char *key_label = NULL;
EVP_PKEY *pKey = NULL;

void load_engine()
{
	ENGINE_load_dynamic();
	engine = ENGINE_by_id(engine_name);
	if(engine==NULL)
	{
		cout << "Failed to load " << engine_name << endl;
		exit(1);
	}
}


void load_private_key()
{
	ENGINE_set_default(engine, ENGINE_METHOD_ALL);
	ENGINE_init(engine);
	pKey = ENGINE_load_private_key(engine, key_label, NULL, NULL);
	if(pKey==NULL)
	{
		cout << "Failed to load " << key_label <<endl;
		exit(1);
	}
	cout << key_label << " found." << endl;
}

void freeMemory()
{
	ENGINE_free(engine);
	EVP_PKEY_free(pKey);
}

int main(int argc, char **argv)
{
	if(argc<3)
	{
		cout << "Please pass the engine name and key label as argument." << endl;
		cout << "load_private_key <engine_name> <private_key_label>" << endl;
		exit(1);
	}
	else
	{
		engine_name = new char[sizeof(argv[1])-1];
		key_label = new char[sizeof(argv[2])-1];
		engine_name = argv[1];
		key_label = argv[2];
	}

	load_engine();
	load_private_key();
	freeMemory();
	return 0;
}
