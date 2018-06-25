#pragma once

#include<iostream>
#include<deque>
#include<boost/asio.hpp>
#include<boost/thread.hpp>
#include"socket_message.hpp"
#include"GameMessages.pb.h"
#include"GameMessageWrap.h"

using namespace boost::asio;

class socket_server;

class talk_to_client : public boost::enable_shared_from_this<talk_to_client>
{
public:
	typedef boost::system::error_code error_code;
	typedef boost::shared_ptr<talk_to_client> ptr;

	void start();

	static ptr create(io_service & io_service, socket_server * socket_server);

	//bool started() const;

	ip::tcp::socket & socket();

	std::string read_data();

	void write_data(std::string s);

	bool error() { return error_flag_; }

	void do_close();

private:
	void handle_read_header(const error_code& error);

	void handle_read_body(const error_code& error);

	talk_to_client(io_service & io_service, socket_server * socket_server);

	void delete_from_server();

	ip::tcp::socket socket_;
	socket_server * socket_server_;
	bool error_flag_ = false;
	//bool started_ = false;

	socket_message read_msg_;
	std::deque<socket_message> read_msg_deque_;

	boost::mutex mut_;
	boost::condition_variable data_conditioner_;
};

class socket_server
{
public:
	static socket_server * create(int port = 8008);

	void close();

	std::vector<talk_to_client::ptr> get_connection() const;

	void remove_connection(talk_to_client::ptr p);

	void button_start();

	bool error() const;

	int connection_num() const;
private:
	socket_server(int port);

	void start_accept();

	void handle_accept(talk_to_client::ptr new_connection, const talk_to_client::error_code& error);

	void loop_process();

	ip::tcp::acceptor acceptor_;
	std::vector<talk_to_client::ptr> connections_;
	int connection_num_;

	static io_service* io_service_;

	boost::thread * thread_;
	boost::thread * button_thread_;

	std::mutex delete_mutex_;
	bool error_flag_{ false };

};


