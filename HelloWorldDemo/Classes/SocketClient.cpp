#include"SocketClient.h"
#include<string>
using boost::asio::ip::tcp;
using boost::asio::ip::address;
typedef boost::shared_ptr<talk_to_server> socket_client_ptr;
typedef boost::system::error_code error_code;
socket_client_ptr talk_to_server::start(std::string ip,int port)
{
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::from_string(ip), port);
	socket_client_ptr new_(new talk_to_server(ep));
	new_->start(ep);
	new_->thread_= new std::thread(
		std::bind(static_cast<std::size_t(boost::asio::io_service::*)()>(&boost::asio::io_service::run),
			&new_->io_service_));
	return new_;
}

void talk_to_server::start(boost::asio::ip::tcp::endpoint ep)
{
	error_flag_ = false;
	socket_.async_connect(ep, std::bind(&talk_to_server::on_connect, this,
		std::placeholders::_1));
}

void talk_to_server::on_connect(const error_code & err)
{
	if (!err)
	{
		
		
		std::cout << "connected\n";
		char data[30] = { 0 };//????30????
		error_code error;
		size_t length = socket_.read_some(boost::asio::buffer(data, 30), error);
		if (error || length < 10)
			throw boost::system::system_error(error);
		char header[4 + 1] = "";
		strncat(header, data + 10, 4);//?????????
		total_ = atoi(header);
		camp_ = atoi(data + 14);
		start_flag_ = true;
		boost::asio::async_read(socket_,boost::asio::buffer(read_msg_.data(), socket_message::header_length), 
			std::bind(&talk_to_server::handle_read_header, this,std::placeholders::_1));
	}
	else
	{
		error_flag_ = true;
		std::cerr << "failed to connect" << std::endl;
	}
}

void talk_to_server::handle_read_header(const error_code & error)
{
	if (!error && read_msg_.decode_header())
	{
		boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
			std::bind(&talk_to_server::handle_read_body, this,
				std::placeholders::_1));
	}
	else
	{
		do_close();
	}
}

void talk_to_server::handle_read_body(const error_code& error)
{
	if (!error)
	{
		std::lock_guard<std::mutex> lk{ mut };//????????????
		//std::condition_variable对象通常使用std::unique_lock<std::mutex>来等待，
		read_msg_deque_.push_back(read_msg_);
		//条件变量std::condition_variable用于多线程之间的通信，它可以阻塞一个或同时阻塞多个线程。std::condition_variable需要与std::unique_lock配合使用。
		//notify_one：唤醒某个wait线程，如果当前没有等待线程，则该函数什么也不做；如果同时存在多个等待线程，则唤醒某个线程是不确定的(unspecified)。
		data_cond_.notify_one();
		std::cout << "read completed\n";

		boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.data(), socket_message::header_length),
			std::bind(&talk_to_server::handle_read_header, this,
				std::placeholders::_1));
	}
	else
	{
		do_close();
	}
}

void talk_to_server::do_write_string(std::string s)
{
	if (error_flag_) return;
	socket_message msg;
	if (s.size() == 0)
	{
		s = std::string("\0");
		msg.set_body_length(1);
	}
	else
		msg.set_body_length(s.size());
	memcpy(msg.body(), &s[0u], msg.body_length());
	msg.encode_header();
	boost::asio::write(socket_,
		boost::asio::buffer(msg.data(), msg.length()));
}

std::string talk_to_server::get_string_from_server()
{
	if (error_flag_)
		return "";
	std::unique_lock<std::mutex> lk{ mut };
	while (read_msg_deque_.empty())
		data_cond_.wait(lk);
	auto read_msg = read_msg_deque_.front();
	read_msg_deque_.pop_front();
	lk.unlock();   
	auto ret = std::string(read_msg.body(), read_msg.body_length());
	return ret;
}

void talk_to_server::do_close()
{
	try 
	{
		std::lock_guard<std::mutex> lk{ mut };
		error_flag_ = true;
		socket_message empty_msg;
		memcpy(empty_msg.data(), "0001\0", 5);
		read_msg_deque_.push_back(empty_msg);
		data_cond_.notify_one();
		io_service_.stop();
		error_code ec;
		socket_.shutdown(tcp::socket::shutdown_both, ec);
		if (!ec)
			throw	boost::system::system_error(ec);
		socket_.close();
		thread_->join();
	}
	catch (std::exception&e)
	{
		e.what();//???????what???
	}

}

void talk_to_server::close()
{
	do_close();
}

void talk_to_server::do_ask_camp()const
{
	//boost::asio::write(socket_, boost::asio::buffer("camp"),ta
}
void talk_to_server::do_read_camp()
{

}