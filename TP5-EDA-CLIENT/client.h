#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <iostream>
#include <fstream>				//Uso lireria fstream para leer y escribir archivos (aunque aca solo escribamos)

#define MAXTOKENS 30
using namespace std; 

typedef struct 
{
	char* memory;
	size_t size;
	size_t nmemb;

}userData;

class Client
{
public:
	Client();
	~Client();
	void curl_initial_set(void);
	CURLcode getErr();
	void setErr(CURLcode err);
	CURL* getCurlhand();
	userData userFileData;
	bool storeMyFile(void);
	bool checkCommand(int argc_, char* argv);

private:
	CURLcode error;
	CURL* curl_handler;
	char* host;
	char* path;
	char* filename;
};

#endif //CLIENT_H