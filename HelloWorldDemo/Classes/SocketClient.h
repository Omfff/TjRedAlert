#pragma once
#include<boost/asio/io_service.hpp>  
#include<boost/asio/ip/tcp.hpp>  
#include<boost/bind.hpp>  
#include<boost/shared_ptr.hpp>  
#include<boost/enable_shared_from_this.hpp>  
#include<string>  
#include<iostream>  
#include <thread>
#include<boost/asio/streambuf.hpp>  
#include<boost/asio/placeholders.hpp>  
#include<boost/asio.hpp> 
#include "socket_message.hpp"
#include<deque>
using boost::asio::ip::tcp;
using boost::asio::ip::address;
class talk_to_server :public boost::enable_shared_from_this<talk_to_server>
{
public:
	typedef boost::shared_ptr<talk_to_server> socket_client_ptr;
	typedef boost::system::error_code error_code;
	static socket_client_ptr start(std::string ip= "127.0.0.1", int port= 8008);
	talk_to_server(boost::asio::ip::tcp::endpoint ep)
		: socket_(io_service_), endpoint_(ep) {}
	void start(boost::asio::ip::tcp::endpoint ep);
	void on_connect(const error_code & err);
	void handle_read_header(const error_code& error);
	void handle_read_body(const error_code& error);
	void do_write_string(std::string s);
	std::string get_string_from_server();
	void do_close();
	void close();
	bool error()const { return error_flag_; }
	bool started() const { return start_flag_; }
	int getCamp()const { return camp_; }
	int getTotalPlayer()const { return total_; }

	void do_ask_camp()const;
	void do_read_camp();
	/*size_t read_complete(const boost::system::error_code &err, size_t bytes);
	void on_connect(const error_code & err);
	void on_read(const error_code & err, size_t bytes);
	void on_login();
	void on_write(const error_code & err, size_t bytes);
	void do_read();
	void do_write(const std::string & msg);
	void handle_connect(const boost::system::error_code &error);
	void handle_write(const boost::system::error_code& error, size_t bytes_transferred);
	void handle_read(const boost::system::error_code& error, size_t bytes_transferred);*/
private:
	bool error_flag_=false;
	bool start_flag_=false;
	boost::asio::io_service io_service_;
	tcp::socket socket_;
	tcp::endpoint endpoint_;
	char buf[1024];
	boost::asio::streambuf sbuf;
	socket_message read_msg_;

	std::condition_variable data_cond_;
	std::mutex mut;
	std::deque<socket_message> read_msg_deque_;

	std::thread *thread_, *read_thread_;

	int total_;
	int camp_;
};
