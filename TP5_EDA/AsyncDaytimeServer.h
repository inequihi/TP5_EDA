#pragma once

/*
	Asynchronous TCP Daytime Server
	Based on:
	https://www.boost.org/doc/libs/1_72_0/doc/html/boost_asio/tutorial/tutdaytime3/src.html
	Example use:
		boost::asio::io_context io_context;
		AsyncDaytimeServer s(io_context);
		s.start();
		io_context.run();
	Tested with client from:
	https://www.boost.org/doc/libs/1_72_0/doc/html/boost_asio/tutorial/tutdaytime1/src.html
*/



#include <boost\asio.hpp>
#include <string>
#include <boost\asio\ip\address.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <fstream>
#include <cstdio>



#define MAX 100
enum result { NO, YES};

class AsyncDaytimeServer
{
public:
	AsyncDaytimeServer(boost::asio::io_context& context);
	~AsyncDaytimeServer();

	void start();
	
private:
	void wait_connection();
	void answer(unsigned int y_n);
	void connection_received_cb(const boost::system::error_code& error);
	void response_sent_cb(const boost::system::error_code& error, size_t bytes_sent);
	void inputHandler(const boost::system::error_code& err, std::size_t bytes_transferred);
	void server_Output(unsigned int y_n);
	std::string msg;
		//TAL VEZ USAR BOOST::ARRAY PARA client input
	char ClientInput[1000];
	size_t FileLenght; 
	bool flag;
	std::string ClientInputStr;
	boost::asio::io_context& context_;
	boost::asio::ip::tcp::socket socket_;
	boost::asio::ip::tcp::acceptor acceptor_;

};