#include "client.h"

size_t write_callback(char* contents, size_t size, size_t numbermemb, void* userData);


/*     CONSTRUCTOR   */


Client::Client()
{
	 setErr(curl_global_init(CURL_GLOBAL_ALL));

	if (getErr() == CURLE_OK)
	{
		curl_handler = curl_easy_init();
		if (curl_handler)
		{
			curl_initial_set();
			userFileData.memory = (char*)malloc(1000);
			userFileData.size = 1;
			userFileData.nmemb = 1000;
		}
	}
}


/*     DESTRUCTOR   */

Client::~Client()
{
	curl_easy_cleanup(curl_handler);
	curl_global_cleanup();
}



/*     SETTER   */



void Client::setErr(CURLcode err) { error = err; };   


/*     GETTER   */ 


CURLcode Client::getErr() { return error; };  
CURL* Client::getCurlhand() { return curl_handler; };



/*     FUNCIONES MIEMBRO   */




void Client::curl_initial_set()
{
	curl_easy_setopt(curl_handler, CURLOPT_URL, "25.135.151.200");			
	curl_easy_setopt(curl_handler, CURLOPT_PORT, 13);				//Escuchamos puerto 80
	curl_easy_setopt(curl_handler, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(curl_handler, CURLOPT_PROTOCOLS,CURLPROTO_HTTP);
	curl_easy_setopt(curl_handler, CURLOPT_WRITEFUNCTION, &write_callback);	//Mando toda la data recibida a funcion callback 
	curl_easy_setopt(curl_handler, CURLOPT_WRITEDATA, (void*)&userFileData);	//Paso la data guardada por callback en this->userData a donde???
}

bool Client::checkCommand(int argc_, char* arguments)
{
	if (argc_ == 2)
	{
		char* tokens[MAXTOKENS] = {  };			//Array temporal para host/path/filename 
		char* tokenPtr = std::strtok(arguments, "/");
		int i, totalTokens = 0;
		while (tokenPtr != NULL && totalTokens < MAXTOKENS)
		{
			tokens[totalTokens] = tokenPtr;
			tokenPtr = std::strtok(NULL, "/");
		}

		this->host = tokens[0];
		this->filename = tokens[totalTokens];
		this->path = tokens[2];
		for (i = 3; i < totalTokens; i++)
		{
			std::strcat(this->path, tokens[i]);
			std::strcat(this->path, "/");
		}

		cout << this->host << endl;
		cout << this->path << endl;
		cout << this->filename << endl;

		return true;
	}
	else 
		return false;
}


bool Client::storeMyFile(void)
{
	ofstream MyFile("myData.txt",ios::binary);		//creo archivo 
	
	if (MyFile.is_open())		//verifico si se creo y puedo abrirlo
	{
		MyFile.write(this->userFileData.memory, this->userFileData.size);		//escribo lo que me devolvio server en un archivo

		MyFile.close();
		cout << "ARCHIVO txt CREADO Y GUARDADO" << endl;
		return true;
	}
	else
	{
		cout << "ERROR CREANDO ARCHIVO txt" << endl; 
		return false;
	}	
}


/*COMENTARIO  --> copie la del ej getinmemory.c pero desps podemos armar una buena con <fstream>*/



size_t write_callback(char* contents, size_t size, size_t cantmemb, void* CurrentUserData)
{
	size_t realsize = size * cantmemb;
	userData* mem = (userData*) CurrentUserData;

	char* ptr = (char*) realloc(mem->memory, mem->size + realsize + 1);
	if (ptr == NULL) {
		/* out of memory! */
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}