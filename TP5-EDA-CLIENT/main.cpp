#include "client.h"

int main(int argc, char** argv)		//Por comando recibo host/path/filename
{
	Client client(argc, argv[1]);	
	if (!(client.getErr()))
	{
		if (client.getCurlhand())
		{
			if (!(curl_easy_perform(client.getCurlhand())))			//Hacer wrapper dentro de client para hacer esto usuario no tiene porque saber de boost
			{
				//Leemos y usamos resultados
				if (client.storeMyFile())
				{
				}
			}
			else
				curl_easy_cleanup(client.getCurlhand());		//Hubo un error asi que libero handler
		}
	}

	return 0;
}