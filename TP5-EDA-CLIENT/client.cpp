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
				userFileData.memory = (char*)malloc(10000);		//Reservamos memoria para informacion recibida por server
				userFileData.size = 1;

			}
		}
		this->error = CURLE_OK;
	}
	else
	{
		this->error = (CURLcode)CURLE_NOTOK;
		cout << "Error en client command input" << this->error << endl;
	}
}


/*     DESTRUCTOR   */

Client::~Client()
{
	curl_global_cleanup();
}

/*     SETTER   */

void Client::setErr(CURLcode err) { error = err; };   


/*     GETTERS   */ 


CURLcode Client::getErr() { return error; };  
CURL* Client::getCurlhand() { return curl_handler; };



/*     FUNCIONES MIEMBRO   */


void Client::curl_initial_set()
{
	curl_easy_setopt(curl_handler, CURLOPT_URL,this->Cmd4Server.c_str());
	curl_easy_setopt(curl_handler, CURLOPT_PORT, 80);				//Escuchamos puerto 80
	curl_easy_setopt(curl_handler, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(curl_handler, CURLOPT_PROTOCOLS, CURLPROTO_HTTP);
	curl_easy_setopt(curl_handler, CURLOPT_WRITEDATA, (void*)&userFileData);	//Paso la data guardada por callback en this->userData a donde???
	curl_easy_setopt(curl_handler, CURLOPT_WRITEFUNCTION, &write_callback);	//Mando toda la data recibida a funcion callback 

}

bool Client::checkCommand(int argc_, char* arguments_)
{
	bool ChkResult = true;
	if (argc_ == 2)		//Si tenemos mas de dos argumentos el input por comando es erroneo
	{
		std::string arguments;
		arguments = arguments_;

		//Buscamos posiciones de el primer y ultimo '/' en el input para separar host de /path/filename
		size_t firstSlash, totalChars, lastSlash;
		firstSlash = arguments.find('/');
		totalChars = arguments.size();

		if (firstSlash != std::string::npos)		//Si recibimos path/file proseguimos
		{
			//Separamos host del path
			this->host.assign(arguments, 0, firstSlash);					//Asignamos host a la clase
			this->path.assign(arguments, firstSlash + 1, totalChars - firstSlash + 1);   //Le cargo path a la clase

			lastSlash = arguments.find_last_of('/');
			this->filename.assign(arguments, lastSlash + 1, totalChars);
			
			//El server recibira "host/path/filename"
			this->Cmd4Server = this->host + '/' + this->path;
		}
		else
			ChkResult = false;
	}
	else
		ChkResult = false;

	return ChkResult;
}
	

bool Client::storeMyFile(void)
{
	std::ofstream MyFile;
	MyFile.open(this->filename.c_str(), ios::binary);		//creo archivo 

	if (MyFile.is_open())		//verifico si se creo y puedo abrirlo
	{
		MyFile.write(this->userFileData.memory, this->userFileData.size);		//escribo lo que me devolvio server en un archivo
		cout << this->userFileData.memory << endl;
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