//
// io_service_pool.cpp
// ~~~~~~~~~~~~~~~~~~~
#include "../include/io_service_pool.h"
#include <stdexcept>
#include <boost/thread/thread.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
io_service_pool::io_service_pool(std::size_t pool_size)
  : next_io_service_(1)
{
	if (pool_size <=1)
		throw std::runtime_error("io_service_pool size is 0");

	// Give all the io_services work to do so that their run() functions will not
	// exit until they are explicitly stopped.
	for (std::size_t i = 0; i < pool_size; ++i)
	{
		io_service_ptr io_service(new boost::asio::io_service);
		work_ptr work(new boost::asio::io_service::work(*io_service));
		io_services_.push_back(io_service);
		work_.push_back(work);
	}
}

void io_service_pool::run(bool bMain)
{
	// Create a pool of threads to run all of the io_services.
	std::vector<boost::shared_ptr<boost::thread> > threads;
	for (std::size_t i = 0; i < io_services_.size(); ++i)
	{
		boost::shared_ptr<boost::thread> thread(new boost::thread(
			  boost::bind(&boost::asio::io_service::run, io_services_[i])));
		threads.push_back(thread);
		std::cout <<"thread id = " << thread->get_id()<< " init\n";
	}

	if (bMain) {
		// Wait for all threads in the pool to exit.
		for (std::size_t i = 0; i < threads.size(); ++i) {
			threads[i]->join();
		}
	}

}

void io_service_pool::stop()
{
	// Explicitly stop all io_services.
	for (std::size_t i = 0; i < io_services_.size(); ++i)
		io_services_[i]->stop();
}

size_t io_service_pool::get_io_service_pool_size() const {
	return io_services_.size();
}

size_t io_service_pool::get_current_io_service_index() const {
	boost::mutex::scoped_lock lock(m_mutex);
	return next_io_service_;
}

boost::asio::io_service& io_service_pool::get_io_service()
{
	boost::mutex::scoped_lock lock(m_mutex);

	// Use a round-robin scheme to choose the next io_service to use.
	boost::asio::io_service& io_service = *io_services_[next_io_service_];
	++next_io_service_;
	if (next_io_service_ == io_services_.size())
		next_io_service_ = 1;
	return io_service;
}

boost::asio::io_service& io_service_pool::get_acceptor_io_service()
{
	boost::mutex::scoped_lock lock(m_mutex);

	// Use a round-robin scheme to choose the next io_service to use.
	boost::asio::io_service& io_service = *io_services_[0];
	return io_service;
}
boost::asio::io_service& io_service_pool::get_io_service(int index)
{
	boost::mutex::scoped_lock lock(m_mutex);
	
	if(index >= io_services_.size())
	{
		return *io_services_[index - 1];
	}

	return *io_services_[index];
}
