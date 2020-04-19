#include "AsyncDaytimeServer.h"

#include <iostream>
#include <string>
#include <boost\bind.hpp>


using  boost::asio::ip::tcp;
using  boost::asio::ip::address;

std::string make_daytime_string();


AsyncDaytimeServer::AsyncDaytimeServer(boost::asio::io_context& io_context)
	: context_(io_context),
	acceptor_(io_context, tcp::endpoint(address::from_string("25.135.150.125"), 80)),
	socket_(io_context)
{
	
	
}


AsyncDaytimeServer::~AsyncDaytimeServer()
{

}

void AsyncDaytimeServer::start()
{
	if (socket_.is_open())
	{
		socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
		socket_.close();
	}
	wait_connection();
}

void AsyncDaytimeServer::wait_connection()
{
	std::cout << "wait_connection()" << std::endl;
	if (socket_.is_open())
	{
		std::cout << "Error: Can't accept new connection from an open socket" << std::endl;
		return;
	}
	acceptor_.async_accept(
		socket_,
		boost::bind(
			&AsyncDaytimeServer::connection_received_cb,
			this,
			boost::asio::placeholders::error
			)
		);
}

void AsyncDaytimeServer::answer()
{
	std::cout << "answer()" << std::endl;
	//msg = make_daytime_string();

	std::fstream fileFromServer("WantThisFile.txt",std::ios::in);			

	if (!fileFromServer.is_open())
	{
		/* https://stackoverflow.com/questions/2912520/read-file-contents-into-a-string-in-c */

		msg.assign((std::istreambuf_iterator<char>(fileFromServer)),
			(std::istreambuf_iterator<char>()));
		msg.append("\r\n\r\n");

		boost::asio::async_write(
			socket_,
			boost::asio::buffer(msg),
			boost::bind(
				&AsyncDaytimeServer::response_sent_cb,
				this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
				)
			);

		fileFromServer.close();
		socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
		socket_.close();

	}
	else
	{
		server_Output(NO);
	}

}

void AsyncDaytimeServer::checkClientInput(const boost::system::error_code& err,
	std::size_t bytes_transferred)
{
	if (!err)
	{
		std::cout << "Error en client input \n";
	}
	else
	{				//Verificamos que se enviaron los comandos validos guardados en ClientInput[]
		char validInput[] = "GET/hamachi/WantThisFile.txt HTTP/1.1\r\nHost: 25.135.158.40\r\n";
		if (strcmp(ClientInput, validInput))
		{
			//Si el input fue correcto vamos a mandarle el archivo al Client
			
			answer();
			server_Output(YES);
		}
		else
		{
			server_Output(NO);
		}
		

	}

}

void AsyncDaytimeServer::server_Output(unsigned int y_n)
{
	std::string ServerOutput; 
	switch (y_n)
	{
	case YES:
		break;
	case NO:
		//SE VE FEO PERO FALTA AGREGAR DATE Y AHI CON LOS + SE PODRIA SEPARAR EN 3 LINEAS 
		ServerOutput = "HTTP/1.1 404 Not Found\r\nDate: AGREGAR DATE \r\nCache-Control: public, max-age=30\r\nExpires: DATE MAS 30 SEGUNDOS \r\nContent-Length: 0\r\nContent-Type: text/html; charset=iso-8859-1\r\n\r\n";

		break;
	}
}
void AsyncDaytimeServer::connection_received_cb(const boost::system::error_code& error)
{
	std::cout << "connection_received_cb()" << std::endl;
	if (!error) {
		boost::asio::async_read(
			socket_,
			boost::asio::buffer(ClientInput),					//Buffer guarda temrinando en \0
			boost::bind(&AsyncDaytimeServer::checkClientInput,
				this,
				boost::asio::placeholders::error, 
				boost::asio::placeholders::bytes_transferred)
			);		//Se llama a checkClientInput despues de guardar en buffer lo recibido

		//answer();
		//wait_connection();		NO ESTOY SEGURA SI ACA TENDRIA Q COMENTAR ESTO
	}
	else {
		std::cout << error.message() << std::endl;
	}
}

void AsyncDaytimeServer::response_sent_cb(const boost::system::error_code& error,
	size_t bytes_sent)
{
	std::cout << "response_sent_cb()" << std::endl;
	if (!error) {
		std::cout << "Response sent. " << bytes_sent << " bytes." << std::endl;
	}

}


std::string make_daytime_string()
{	
char CR = 0x0D;
	char LF = 0x0A;
	std::string msg1("HTTP/11.1 404 NOT FOUND ");
	std::string msg2("DATE: CRLF");
	std::string msg3("CACHE-CONTROL CRLF");
	std::string msg4("EXPIRES: CRLF");
	std::string msg5("LENGTH CRLF");
	std::string msg6("type CRLF");


	msg1 += CR;
	msg1 += LF;

	msg2 += CR;
	msg2 += LF;

	msg3 += CR;
	msg3 += LF;

	msg4 += CR;
	msg4 += LF;


	msg5 += CR;
	msg5 += LF;

	msg6 += CR;
	msg6 += LF;

	return msg1 + msg2 + msg3 + msg4 + msg5 + msg6;
}