#include"SocketServer.h"

io_service * socket_server::io_service_ = new io_service;

void talk_to_client::start()
{
	async_read(socket_, buffer(read_msg_.data(), socket_message::header_length), 
		boost::bind(&talk_to_client::handle_read_header, this, _1));
}

talk_to_client::ptr talk_to_client::create(io_service & io_service, socket_server * socket_server)
{
	return talk_to_client::ptr(new talk_to_client(io_service, socket_server));
}

ip::tcp::socket & talk_to_client::socket()
{
	return socket_;
}

std::string talk_to_client::read_data()
{
	if (error_flag_) 
	{
		return "";
	}
	boost::unique_lock<boost::mutex> lk{ mut_ };
	while (read_msg_deque_.empty()) 
	{
		data_conditioner_.wait(lk);
	}
	auto read_msg = read_msg_deque_.front();
	read_msg_deque_.pop_front();
	lk.unlock();
	auto ret = std::string(read_msg.body(), read_msg.body_length());
	return ret;
}

void talk_to_client::write_data(std::string s)
{
	if (error_flag_) 
	{
		return;
	}
	socket_message msg;
	if (s.size() == 0)
	{
		s = std::string("\0");
		msg.set_body_length(1);
	}
	else 
	{
		msg.set_body_length(s.size());
	}
	memcpy(msg.body(), &s[0u], msg.body_length());
	msg.encode_header();
	write(socket_,buffer(msg.data(), msg.length()));
}

void talk_to_client::do_close()
{
	try
	{
		//		steady_timer_.cancel();
		error_flag_ = true;
		socket_message empty_msg;
		memcpy(empty_msg.data(), "0001\0", 5);
		read_msg_deque_.push_back(empty_msg);
		read_msg_deque_.push_back(empty_msg);
		data_conditioner_.notify_one();
		error_code ec;
		socket_.shutdown(ip::tcp::socket::shutdown_both, ec);
		if (!ec)
			throw std::system_error(ec);
		socket_.close();
	}
	catch (std::exception&e)
	{
		e.what();
	}
	delete_from_server();
}

void talk_to_client::handle_read_header(const boost::system::error_code& error)
{
	if (!error && read_msg_.decode_header()) 
	{
		async_read(socket_, buffer(read_msg_.body(), read_msg_.body_length()),
			boost::bind(&talk_to_client::handle_read_body, this, _1));
	}
	else 
	{
		do_close();
	}
}

void talk_to_client::handle_read_body(const boost::system::error_code& error)
{
	if (!error) 
	{
		boost::lock_guard<boost::mutex> lk{ mut_ };
		read_msg_deque_.push_back(read_msg_);
		data_conditioner_.notify_one();
		async_read(socket_, buffer(read_msg_.data(), socket_message::header_length),
			boost::bind(&talk_to_client::handle_read_header, this, _1));
	}
	else 
	{
		do_close();
	}
}

talk_to_client::talk_to_client(io_service & io_service, socket_server * socket_server)
	:socket_(io_service), socket_server_(socket_server)
{
	std::cout << "new tcp" << std::endl;
}

void talk_to_client::delete_from_server()
{
	if (socket_server_) 
	{
		shared_from_this()->socket_server_->remove_connection(shared_from_this());
	}
	socket_server_ = nullptr;
}

socket_server * socket_server::create(int port)
{
	auto s = new socket_server(port);
	s->thread_ = new boost::thread(boost::bind(static_cast<std::size_t(io_service::*)()>(&io_service::run), io_service_));
	return s;
}

void socket_server::close()
{
	try 
	{
		connections_.clear();
		io_service_->stop();
		acceptor_.close();
		thread_->join();
		delete io_service_;
	}
	catch (std::exception&e)
	{
		std::cerr << e.what() << std::endl;
	}
	io_service_ = new io_service;
}

void socket_server::button_start()
{
	acceptor_.close();
	using namespace std;
	char total[5] = "";
	sprintf(total, "%4d", static_cast<int>(connections_.size()));
	for (auto i = 0; i < connections_.size(); i++) 
	{
		connections_[i]->write_data("PLAYER" + std::string(total) + std::to_string(i + 1));
	}
	connection_num_ = connections_.size();
	this->button_thread_ = new boost::thread(std::bind(&socket_server::loop_process, this));
	button_thread_->detach();
}

bool socket_server::error() const
{
	return error_flag_;
}

int socket_server::connection_num() const
{
	return connections_.size();
}

std::vector<talk_to_client::ptr> socket_server::get_connection() const
{
	return connections_;
}

void socket_server::remove_connection(talk_to_client::ptr p)
{
	std::unique_lock<std::mutex> lock(delete_mutex_);
	auto position = std::find(connections_.begin(), connections_.end(), p);

	if (position == connections_.end())
		std::cout << "delete not succ\n";
	else
		connections_.erase(position);
	std::cout << "delete succ\n";
	
}

socket_server::socket_server(int port) : acceptor_(*io_service_, ip::tcp::endpoint(ip::tcp::v4(), port))
{
	start_accept();
}

void socket_server::start_accept()
{
	talk_to_client::ptr new_connection = talk_to_client::create(acceptor_.get_io_service(), this);
	acceptor_.async_accept(new_connection->socket(), boost::bind(&socket_server::handle_accept, this, new_connection, _1));
	std::cout << "start accept " << std::endl;
}

void socket_server::handle_accept(talk_to_client::ptr new_connection, const boost::system::error_code& error)
{
	std::cout << "handle_accept" << std::endl;
	if (!error) 
	{
		connections_.push_back(new_connection);
		std::cout << new_connection->socket().remote_endpoint().address()
			<< ":" << new_connection->socket().remote_endpoint().port() << std::endl;
		new_connection->start();
	}
	start_accept();
}

void socket_server::loop_process()
{
	while (true) 
	{
		if (connections_.size() != connection_num_) 
		{
			error_flag_ = true;
			break;
		}
		std::unique_lock<std::mutex> lock(delete_mutex_);
		std::vector<std::string> ret;
		for (auto r : connections_) 
		{
			if (r->error()) 
			{
				error_flag_ |= r->error();
			}
			ret.push_back(r->read_data());
		}
		auto game_msg = GameMessageWrap::combine_message(ret);
		for (auto r : connections_)
			r->write_data(game_msg);
	}
}



