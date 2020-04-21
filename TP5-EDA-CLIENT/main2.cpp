#include "client.h"

int main(int argc, char** argv)		//Por comando recibo host/path/filename
{
	Client client(argc, argv[1]);	

	if (client.getCurlhand())
		{
			if (!(curl_easy_perform(client.getCurlhand())))
			{
				//Leemos y usamos resultados
				if (client.storeMyFile())
				{
					printf(" \n\n ente \n\n");
				}
				printf(" \n\n sali \n\n");
			}
			else
				curl_easy_cleanup(client.getCurlhand());		//Hubo un error asi que libero handler
		}

	client.~Client();

	return 0;
}