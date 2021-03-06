#pragma once
#include <boost\asio.hpp>
#include <string>
#include <boost\asio\ip\address.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <streambuf>
#include <time.h>




#define SIZE 26
#define MAX 100
enum result { NO, YES};

class AsyncDaytimeServer
{
public:
	AsyncDaytimeServer(boost::asio::io_context& context);
	~AsyncDaytimeServer();

	void start();

	std::string makeDaytimeString(int secs);



private:
	void wait_connection();
	void answer();
	void connection_received_cb(const boost::system::error_code& error);
	void response_sent_cb(const boost::system::error_code& error, size_t bytes_sent);
	void inputHandler(const boost::system::error_code& err, std::size_t bytes_transferred);
	void server_Output(unsigned int y_n);
	std::string msg;
	std::string date;
	std::string ServerOutput;


	char buf[SIZE];
	char ClientInput[1000];
	size_t FileLenght; 
	bool flag;
	std::string ClientInputStr;
	boost::asio::io_context& context_;
	boost::asio::ip::tcp::socket socket_;
	boost::asio::ip::tcp::acceptor acceptor_;

};