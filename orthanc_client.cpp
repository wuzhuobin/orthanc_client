#include "orthanc_client.h"
// std 
#include <iostream>
// boost asio
// #include <boost/asio/io_context.hpp>
// #include <boost/asio/ip/tcp.hpp>
// #include <boost/asio/placeholders.hpp>
// #include <boost/asio/streambuf.hpp>
// #include <boost/asio/write.hpp>
#include <boost/asio.hpp>
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
        os << "GET " << self->path << " HTTP/1.1\r\n";
        os << "Host: " << self->server << ":" << self->port << "\r\n";
        // os << "Accept: application/dicom\r\n";
        os << "Accept: */*\r\n";
        // os << "Connection: close\r\n";
        os << "\r\n";

        std::cerr << "GET " << self->path << " HTTP/1.1\r\n";
        std::cerr << "Host: " << self->server << ":" << self->port << "\r\n";
        // std::cerr << "Accept: application/dicom\r\n";
        std::cerr << "Accept: */*\r\n";
        std::cerr << "Connection: close\r\n";
        std::cerr << "\r\n";
    }
    void handle_resolve(
        const boost::system::error_code &error, 
        const boost::asio::ip::tcp::resolver::results_type &results)
    {
        if (error)
        {
            std::cerr << "Error: " << error.category().name() << ' ' <<
                error.message() << ' ' << error.value() << '\n';
            return;
        }
        // std::cerr << "server: " << this->self->server << '\n';
        // std::cerr << "port: " << this->self->port << '\n';
        // std::cerr << "path:" << this->self->path << '\n';
        // boost::asio::async_connect(this->socket, results.begin(), results.end(),
        //     boost::bind(&orthanc_client_private::handle_connect, this, 
        //         boost::asio::placeholders::error)
        boost::asio::async_connect(this->socket, results,
            boost::bind(&orthanc_client_private::handle_connect, this, 
                boost::asio::placeholders::error)
        );
    }

    void handle_connect(const boost::system::error_code &error)
    {
        if (error)
        {
            std::cerr << "Error: " << error.category().name() << ' ' <<
                error.message() << ' ' << error.value() << '\n';
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

    void handle_request(const boost::system::error_code &error, const std::size_t &bytes)
    {
        if(error){
            std::cerr << "Error: " << error.category().name() << ' ' <<
                error.message() << ' ' << error.value() << '\n';
            return;
        }
        // boost::asio::async_read(this->socket, this->response,
        //     boost::bind(&orthanc_client_private::handle_response, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
        // );
        boost::asio::async_read_until(this->socket, this->response, "\r\n",
                                      boost::bind(&orthanc_client_private::handle_response, this,
                                                  boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    
    void handle_response(const boost::system::error_code &error, const std::size_t &bytes)
    {
        if(error){
            std::cerr << "Error: " << error.category().name() << ' ' <<
                error.message() << ' ' << error.value() << '\n';
            return;
        }
        std::cerr << &response;
        // boost::asio::async_read(this->socket, this->response,
        //     boost::bind(&orthanc_client_private::handle_response, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
        // );
    }

    orthanc_client *self;
    boost::asio::ip::tcp::resolver resolver;
    boost::asio::ip::tcp::socket socket;
    boost::asio::streambuf request;
    boost::asio::streambuf response;
};

orthanc_client::orthanc_client(
    const std::string &server, 
    const std::string &port,
    const std::string &path) :
    server(server),
    port(port),
    path(path),
    io_context(new boost::asio::io_context),
    p(new orthanc_client_private(this))
{
   std::cerr << "Before run. \n";  
   this->io_context->run();
   std::cerr << "Running. \n";
}
