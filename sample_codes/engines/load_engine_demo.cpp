/*
	- Thanks for reading DISCLAIMER.txt
	This code demonstrates how to load engines using OpenSSL API.
*/

#include <iostream>
#include <openssl/engine.h>

using namespace std;


int main(int argc, char **argv)
{
	char *engine_name = NULL;
	ENGINE *engine = NULL;

	if(argc<2)
	{
		cout << "Engine name argument missing..." << endl;
		cout << "load_engine_demo <engine_name>" << endl;
		exit(1);
	}
	else
	{
		engine_name = new char[sizeof(argv[1])-1];
		engine_name = argv[1];
	}

	ENGINE_load_dynamic();
	engine = ENGINE_by_id(engine_name);

	if(engine!=NULL) 
	{
		cout << engine_name << " loaded successfully." << endl;
	}
	else
	{
		cout << "Failed to load " << engine_name << endl;
		exit(1);
	}

	ENGINE_free(engine);
	return 0;
}
