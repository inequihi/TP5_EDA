#include "client.h"

size_t write_callback(char* contents, size_t size, size_t numbermemb, void* userData);


/*     CONTRUCTOR   */


Client::Client()
{
	 setErr(curl_global_init(CURL_GLOBAL_ALL));

	if (getErr() == CURLE_OK)
	{
		curl_handler = curl_easy_init();
		if (curl_handler)
		{
			curl_initial_set();
			userData_.memory = (char*)malloc(1000);
			userData_.size = 1;
			userData_.nmemb = 1000;
		}
	}
}


/*     DESTRUCTOR   */

Client::~Client()
{
	curl_easy_cleanup(curl_handler);
}



/*     SETTER   */



void Client::setErr(CURLcode err) { error = err; };   


/*     GETTER   */ 


CURLcode Client::getErr() { return error; };  
CURL* Client::getCurlhand() { return curl_handler; };



/*     FUNCIONES MIEMBRO   */




void Client::curl_initial_set()
{
	curl_easy_setopt(curl_handler, CURLOPT_URL, "25.135.151.200");			//NO ENTIENDO QUE SERIA LOCAL HOST tipo Host: 127.0.0.1 CRLF ... CRLF ?
	curl_easy_setopt(curl_handler, CURLOPT_PORT, 13);				//Escuchamos puerto 80
	curl_easy_setopt(curl_handler, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(curl_handler, CURLOPT_PROTOCOLS,CURLPROTO_HTTP);
	curl_easy_setopt(curl_handler, CURLOPT_WRITEFUNCTION, &write_callback);	//Mando toda la data recibida a funcion callback 
	curl_easy_setopt(curl_handler, CURLOPT_WRITEDATA, (void*)&userData_);	//Paso la data guardada por callback en this->userData a donde???
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