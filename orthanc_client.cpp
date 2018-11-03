#include "orthanc_client.h"
// std 
#include <iostream>
// boost asio
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>
// boost bind 
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>
struct orthanc_client_private
{
    orthanc_client_private(orthanc_client *self):
        self(self),
        resolver(*self->io_context), 
        socket(*self->io_context)
    {
        this->resolver.async_resolve(self->server, self->port, 
            boost::bind(&orthanc_client_private::handle_resolve,
            this, 
            boost::asio::placeholders::error, 
            boost::asio::placeholders::results)
        );
        std::ostream os(&request);
        // os << "Get " << self->path << " HTTP/1.1\r\n";
    }
    void handle_resolve(
        const boost::system::error_code &error, 
        const boost::asio::ip::tcp::resolver::results_type &results)
    {
        if (error)
        {
            std::cerr << "Error: " << error.category().name() << ' ' << error.value() << '\n';
            return;
        }
        this->socket.async_connect(*results,
            boost::bind(&orthanc_client_private::handle_connect, this, 
                boost::asio::placeholders::error)
        );
    }

    void handle_connect(const boost::system::error_code &error)
    {
        if(error){
            std::cerr << "Error: " << error.category().name() << ' ' << error.value() << '\n';
            return;
        }
        // this->socket.async_write_some(this->request, 
        //     boost::bind(&orthanc_client_private::handle_request, this, 
        //     boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
        // );
        boost::asio::async_write(this->socket, this->request, 
            boost::bind(&orthanc_client_private::handle_request, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
        );
    }

    void handle_request(const boost::system::error_code &error, std::size_t bytes)
    {
        if(error){
            std::cerr << "Error: " << error.category().name() << ' ' << error.value() << '\n';
            return;
        }

    }

    orthanc_client *self;
    boost::asio::ip::tcp::resolver resolver;
    boost::asio::ip::tcp::socket socket;
    boost::asio::streambuf request;
};

orthanc_client::orthanc_client(
    const std::string &server, 
    const std::string &port,
    const std::string &path) :
    server(server),
    path(path),
    p(new orthanc_client_private(this))
{

}
