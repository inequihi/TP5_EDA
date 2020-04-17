#include "client.h"

int main(void)
{
	Client client;

	if(client.getCurlhand())
	{
	

		if (curl_easy_perform(client.getCurlhand())) 
		{

			printf("ERROR:");

		}
	}


		
	curl_global_cleanup();
	return 0;
}