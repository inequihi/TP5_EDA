#include "AsyncDaytimeServer.h"

#include <iostream>
#include <string>
#include <boost\bind.hpp>


using  boost::asio::ip::tcp;
using  boost::asio::ip::address;

std::string make_daytime_string();


AsyncDaytimeServer::AsyncDaytimeServer(boost::asio::io_context& io_context)
	: context_(io_context),
	acceptor_(io_context, tcp::endpoint(address::from_string("25.135.151.200"), 80)),
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

void AsyncDaytimeServer::connection_received_cb(const boost::system::error_code& error)
{
	std::cout << "connection_received_cb()" << std::endl;
	if (!error) {
		/* http://charette.no-ip.com:81/programming/doxygen/boost/group__async__read.html */
		

		socket_.async_receive(
			
			boost::asio::buffer(ClientInput,10000),  //Buffer guarda temrinando en \0
			0,
			boost::bind(&AsyncDaytimeServer::inputHandler,
				this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)
			);	


	}
	else {
		std::cout << error.message() << std::endl;
	}
}

//VER DESPS SI ME CONVIENE PASARLE bytes_transferred A ANSWER()
void AsyncDaytimeServer::inputHandler(const boost::system::error_code& err,
	std::size_t bytes_transferred)
{
	if (!err)
	{

		std::cout << ClientInput << std::endl;
		std::string stringInpt = ClientInput;
		std::size_t found = stringInpt.find("/TP5_EDA/trend.txt", 0);

		//Si no hay error en Client Input 
		if (found != std::string::npos)
		{
			answer(YES);
		}
		else
		{
			answer(NO);
		}
	}
	else
	{
		//ACA CLOSE ??
	}

	wait_connection();		

}

void AsyncDaytimeServer::answer(unsigned int y_n)
{
	std::cout << "answer()" << std::endl <<  std::endl;

	std::fstream fileFromServer("C:/Users/manuc/source/repos/TP5_EDA/TP5_EDA/trend.txt",std::ios::in | std::ios::binary);

	if (!fileFromServer.is_open() && (y_n == YES))
	{
		std::ostringstream msgStream;
		msgStream << fileFromServer.rdbuf();
		msg += msgStream.str();
		FileLenght = msg.length();
		msg.append("\r\n\r\n");
		server_Output(YES);
		fileFromServer.close();
	}
	else
	{
		server_Output(NO);
	}

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

		socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
		socket_.close();

}

void AsyncDaytimeServer::server_Output(unsigned int y_n)
{
	std::string ServerOutput; 
	switch (y_n)
	{
	case YES:
																//NO ESTOY SEGURA SI ESTE CAST ES VALIDO sino usar ClientInputStr
		ServerOutput = "HTTP/1.1 200 OK \r\n Date: FALTA FECHA \r\n Location:" + (std::string)ClientInput + "\r\n" +
			"Cache-Control: max-age=30\r\n" +
			"Expires: FALTA FECHA \r\n" +
			"Content-Lenght" + boost::lexical_cast<std::string>(FileLenght) + "\r\n" +
			"Content-Type: text/html; charset=iso-8859-1\r\n\r\n";

//https://www.boost.org/doc/libs/1_72_0/doc/html/boost_lexical_cast/examples.html#boost_lexical_cast.examples.numbers_to_strings_conversion

		ServerOutput.append(msg);
		//Agrego el mensaje enviado a cliente para imprimir el archivo compartido
		std::cout << ServerOutput << std::endl;
		
		break;
	case NO:
		//SE VE FEO PERO FALTA AGREGAR DATE Y AHI CON LOS + SE PODRIA SEPARAR EN 3 LINEAS 
		ServerOutput = "HTTP/1.1 404 Not Found\r\nDate: AGREGAR DATE \r\nCache-Control: public, max-age=30\r\nExpires: DATE MAS 30 SEGUNDOS \r\nContent-Length: 0\r\nContent-Type: text/html; charset=iso-8859-1\r\n\r\n";
		std::cout << ServerOutput << std::endl;
		break;
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

