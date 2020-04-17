#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>


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

private:
	CURLcode error;
	CURL* curl_handler;
	userData userData_;
};

#endif //CLIENT_H