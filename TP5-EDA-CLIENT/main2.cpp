#include "client.h"

int main(int argc, char** argv)		//Por comando recibo host/path/filename
{
	Client client;	

	if (client.storeMyFile())
	{
		printf("FUNCIONO\n");

	}
	else
		printf("no funciono :( \n");
	

	if (client.getCurlhand())
	{
		if (curl_easy_perform(client.getCurlhand()))
		{
			//Leemos y usamos resultados
			if (client.storeMyFile())
			{

			}
		}
	}
		


		
	client.~Client();

	return 0;
}