#include "AsyncDaytimeServer.h"

#include <iostream>
#include <string>
#include <boost\bind.hpp>


using  boost::asio::ip::tcp;
using  boost::asio::ip::address;



AsyncDaytimeServer::AsyncDaytimeServer(boost::asio::io_context& io_context)
	: context_(io_context),
	acceptor_(io_context, tcp::endpoint(address::from_string("25.135.151.200"), 80)),
	socket_(io_context)
{
	FileLenght = 0;	
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
	std::cout << "connection_received_cb()" << std::endl << std::endl;
	if (!error) {

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

void AsyncDaytimeServer::inputHandler(const boost::system::error_code& err,
	std::size_t bytes_transferred)
{

	if (!err)
	{
		std::cout << "No hay Error en client input " << std::endl << std::endl; // recive bien parametros de ckient 
		
		flag = TRUE;
	}
	else
	{
		flag = FALSE;
	}

	if (flag)
	{				//Verificamos que se enviaron los comandos validos guardados en ClientInput[]


		
		std::string stringInpt = ClientInput;


		std::size_t found = stringInpt.find("/TP5_EDA/trend.txt",0);
		

		if (found != std::string::npos)
		{
			answer();
		}
		else
		{
			server_Output(NO);
			boost::asio::async_write(
				socket_,
				boost::asio::buffer(ServerOutput), // aca poniamos mensaje
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
	}
	wait_connection();		

}

void AsyncDaytimeServer::answer()
{

	std::fstream fileFromServer("trend.txt",std::ios::in | std::ios::binary);

	if (fileFromServer.is_open()) // aca iba sin ! de negado 
	{

		std::ostringstream text;
		text << fileFromServer.rdbuf();
		msg += text.str();
		

		FileLenght = msg.length();


		server_Output(YES);



		boost::asio::async_write(
			socket_,
			boost::asio::buffer(ServerOutput), // aca poniamos mensaje
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
		std::cout << "No se puso abrir archivo" << std::endl;		//El archivo esta (ya que paso la prueba de CheckClientInput) pero no se pudo abrir

	}

}

void AsyncDaytimeServer::server_Output(unsigned int y_n)
{
	std::string hoy = "HOY";
	switch (y_n)
	{
	case YES:
		ServerOutput="HTTP/1.1 200 OK\r\nDate:" + makeDaytimeString(0) + "Location: " + "C:/Users/manuc/source/repos/TP5_EDA/TP5_EDA/trend.txt" + "\r\nCache-Control: max-age=30\r\nExpires:" +
			makeDaytimeString(30) + "Content-Length:" + boost::lexical_cast<std::string>(FileLenght) +
			"\r\nContent-Type: text/html; charset=iso-8859-1\r\n\r\n";


		ServerOutput += msg;
		ServerOutput += "\r\n\r\n";

		//Agrego el mensaje enviado a cliente para imprimir el archivo compartido
		std::cout << ServerOutput << std::endl;
		
		break;
	case NO:

		ServerOutput = "HTTP/1.1 404 Not Found\r\nDate:" + makeDaytimeString(0) + "Cache-Control: public, max-age=30\r\nExpires:" + makeDaytimeString(30) + "Content-Length: 0\r\nContent-Type: text/html; charset=iso-8859-1\r\n\r\n";
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


std::string AsyncDaytimeServer::makeDaytimeString(int secs) { //funcion para devolver el tiempo 


	std::chrono::system_clock::time_point time = std::chrono::system_clock::now();

	time += std::chrono::seconds(secs);

	time_t time_ = std::chrono::system_clock::to_time_t(time);

	//	ctime_s(buf, SIZE, &time_);
	//
	//	date = buf;
	//	date.resize(date.length() - 1);
	//
	//	return date;
	//}
	return ctime(&time_);
}