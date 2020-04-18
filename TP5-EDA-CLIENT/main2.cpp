#include "client.h"

int main(int argc, char** argv)		//Por comando recibo host/path/filename
{
	Client client;	

	if (client.checkCommand(argc, argv[1]))			//Check command debe recibir host/path1/path2/pathetc/filename
	{
		if (client.getCurlhand())
		{
			if (curl_easy_perform(client.getCurlhand()))
			{
				//Leemos y usamos resultados
				//if (client.storeMyFile())
				//{

				//}
			}
			else
				curl_easy_cleanup(client.getCurlhand());		//Hubo un error asi que libero handler
		}
	}



	client.~Client();

	return 0;
}