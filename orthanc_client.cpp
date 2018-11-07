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
#include <boost/filesystem.hpp>
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
        boost::asio::async_write(this->socket, this->request, 
            boost::bind(&orthanc_client_private::handle_request, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
        );
    }

    void handle_request(const boost::system::error_code &error, const std::size_t &bytes)
    {
        if (error)
        {
            std::cerr << "Error: " << error.category().name() << ' ' <<
                error.message() << ' ' << error.value() << '\n';
            return;
        }
        boost::asio::async_read_until(this->socket, this->response, "\r\n\r\n",
            boost::bind(&orthanc_client_private::handle_response, this,
                boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

    void handle_response(const boost::system::error_code &error, const std::size_t &bytes)
    {
        if(!error)
        {
            const char *data = static_cast<const char *>(this->response.data().data());
            std::string header(bytes, '\0');
            std::istream is(&this->response);
            is.read(const_cast<char*>(header.data()), bytes);
            if(header.substr(9, 3) != "200")
            {
                std::cerr << "Reponse status is not 200.\n" << header << '\n'; 
                return;
            }
            boost::asio::async_read(this->socket, this->response,
                boost::bind(&orthanc_client_private::handle_response, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
            );
        }
        else if(error == boost::asio::error::eof)
        {
            // hendle content
            boost::filesystem::path temp = boost::filesystem::current_path() += "/temp";
            boost::filesystem::create_directory(temp);
            boost::filesystem::path dcm = temp;
            dcm += self->path;
            boost::filesystem::ofstream out(dcm, boost::filesystem::ofstream::out);
            out << &this->response;
            out.close();
        }
        else
        {
            std::cerr << "Error: " << error.category().name() << ' ' <<
                error.message() << ' ' << error.value() << '\n';
        }
    }

    orthanc_client *self;
    boost::asio::ip::tcp::resolver resolver;
    boost::asio::ip::tcp::socket socket;
    boost::asio::streambuf request;
    boost::asio::streambuf response;
};

orthanc_client::orthanc_client(
    const std::string &server, 
    const std::string &port) :
    server(server),
    port(port),
    io_context(new boost::asio::io_context),
    this_p(new orthanc_client_private(this))
{
}

void orthanc_client::method_GET(const std::string &path)
{
    this->path = path;
    std::ostream os(&this_p->request);
    os << "GET " << this->path << " HTTP/1.1\r\n";
    os << "Host: " << this->server << ":" << this->port << "\r\n";
    os << "Accept: */*\r\n";
    os << "Connection: close\r\n";
    os << "\r\n";
    std::cerr << static_cast<const char*>(this_p->request.data().data());
    this_p->resolver.async_resolve(this->server, this->port, 
        boost::bind(&orthanc_client_private::handle_resolve,
        this_p, 
        boost::asio::placeholders::error, 
        boost::asio::placeholders::results)
    );
    this->io_context->run();
}