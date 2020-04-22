#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <iostream>
#include <fstream>				//Uso lireria fstream para leer y escribir archivos (aunque aca solo escribamos)

#define MAXTOKENS 30
#define CURLE_NOTOK 1;
using namespace std; 

typedef struct 
{
	char* memory;
	size_t size;
}userData;

class Client
{
public:
	Client(int argc, char* argv_);
	~Client();

	/**
	 * @brief   curl_initial_set: activacion de todos los setting options de Libcurl.
	 **/
	void curl_initial_set(void);

	/**
	 * @brief   getErr: Getter de resultado de curl_global_init 
	 **/
	CURLcode getErr();

	/**
	 * @brief   setErr: Setter de resultado de curl_global_init
	 **/
	void setErr(CURLcode err);

	/**
	* @brief   getCurlhand: Getter de handler de cliente
	 **/
	CURL* getCurlhand();

	/**
	* @brief   storeMyFile: Guarda data recibida por server y la guarda en un archivo en
	*						una carpeta del cliente
	 **/
	bool storeMyFile(void);

	/**
	* @brief   checkCommand: Verifica los comandos recibidos del cliente.
	*						 Prepara el host/path/filename para acceder al server.
	**/
	bool checkCommand(int argc_, char* argv);

private:
	userData userFileData;			//estructura donde se guardara informacion recibida del server
	CURLcode error;					//Codigo de error de inicializaciones de libCurl
	CURL* curl_handler;				//Handler de operaciones del cliente

	std::string Cmd4Server;			//String donde se guardan los comandos a ser enviados al server.
	std::string host;				//Direccion IP del server
	std::string path;				//Direccion del archivo que se pide al server
	std::string filename;
};

#endif //CLIENT_H