#include "client.h"

size_t write_callback(char* contents, size_t size, size_t numbermemb, void* userData);


/*     CONSTRUCTOR   */


Client::Client(int argc_, char* argv_)
{
	if (checkCommand(argc_, argv_))
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

			}
		}
	}

}


/*     DESTRUCTOR   */

Client::~Client()
{
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
	curl_easy_setopt(curl_handler, CURLOPT_URL, (this->Cmd4Server).c_str());
	curl_easy_setopt(curl_handler, CURLOPT_PORT, 80);				//Escuchamos puerto 80
	curl_easy_setopt(curl_handler, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(curl_handler, CURLOPT_PROTOCOLS, CURLPROTO_HTTP);
	curl_easy_setopt(curl_handler, CURLOPT_WRITEDATA, (void*)&userFileData);	//Paso la data guardada por callback en this->userData a donde???
	curl_easy_setopt(curl_handler, CURLOPT_WRITEFUNCTION, &write_callback);	//Mando toda la data recibida a funcion callback 

}

/*
void Client::curl_initial_set()
{
	curl_easy_setopt(curl_handler, CURLOPT_URL, "http://www.policiactes.gov.ar/");			
	curl_easy_setopt(curl_handler, CURLOPT_PORT, 80);				//Escuchamos puerto 80
	curl_easy_setopt(curl_handler, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(curl_handler, CURLOPT_PROTOCOLS,CURLPROTO_HTTP);
	curl_easy_setopt(curl_handler, CURLOPT_WRITEDATA, (void*)&userFileData);	//Paso la data guardada por callback en this->userData a donde???
	curl_easy_setopt(curl_handler, CURLOPT_WRITEFUNCTION, &write_callback);	//Mando toda la data recibida a funcion callback 
}
*/

bool Client::checkCommand(int argc_, char* arguments_)
{
	std::string arguments;


	if (argc_ == 2)
	{
		arguments = arguments_;
		unsigned int firstSlash, lastSlash, totalChars;
		firstSlash = arguments.find('/');			//https://www.cplusplus.com/reference/string/string/find/?kw=string%3A%3Afind
		totalChars = arguments.size() ;
		
		if ( firstSlash != std::string::npos)
		{
			this->host.assign(arguments, 0, firstSlash);					//Le cargo el host a la clase
			this->path.assign(arguments, firstSlash + 1, totalChars - firstSlash + 1);   //Le cargo path a la clase

			this->Cmd4Server = this->host + '/' + this->path;
			
			//cout << this->host << endl << this->path << endl;
		}

		return true;
	}
	else 
		return false;
}
	

bool Client::storeMyFile(void)
{
	std::ofstream MyFile;
	MyFile.open("MyNewData.txt", ios::binary);		//creo archivo 

	if (MyFile.is_open())		//verifico si se creo y puedo abrirlo
	{
		MyFile.write(this->userFileData.memory, this->userFileData.size);		//escribo lo que me devolvio server en un archivo

		MyFile.close();
		return true;
	}
	else
	{
		cout << "ERROR CREANDO ARCHIVO txt" << endl; 
		return false;
	}	
}


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