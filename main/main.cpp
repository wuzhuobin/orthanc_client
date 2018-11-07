#include "orthanc_client.h"
#include <cstdlib>
#include <iostream>
int main(int argc, char *argv[])
{
  orthanc_client client("223.255.146.2", "8042");
  client.method_GET("/orthanc/instances/593b7f4f-a6a19241-e255c4be-3e9fef2c-6d8f96a8/file");
  // client.run();
  // orthanc_client client("theboostcpplibraries.com", "80", '/');
  std::cin.get();
  return EXIT_SUCCESS;
}
// #include <boost/asio/io_service.hpp>
// #include <boost/asio/write.hpp>
// #include <boost/asio/buffer.hpp>
// #include <boost/asio/ip/tcp.hpp>
// #include <array>
// #include <string>
// #include <iostream>

// using namespace boost::asio;
// using namespace boost::asio::ip;

// io_service ioservice;
// tcp::resolver resolv{ioservice};
// tcp::socket tcp_socket{ioservice};
// std::array<char, 4096> bytes;

// void read_handler(const boost::system::error_code &ec,
//   std::size_t bytes_transferred)
// {
//   if (!ec)
//   {
//     std::cout.write(bytes.data(), bytes_transferred);
//     tcp_socket.async_read_some(buffer(bytes), read_handler);
//   }
// }

// void connect_handler(const boost::system::error_code &ec)
// {
//   if (!ec)
//   {
//     std::string r =
//       "GET /orthanc/instances HTTP/1.1\r\nHost: 223.255.146.2\r\n\r\n";
//     write(tcp_socket, buffer(r));
//     tcp_socket.async_read_some(buffer(bytes), read_handler);
//   }
// }

// void resolve_handler(const boost::system::error_code &ec,
//   tcp::resolver::iterator it)
// {
//   if (!ec)
//     tcp_socket.async_connect(*it, connect_handler);
// }

// int main()
// {
//   tcp::resolver::query q{"223.255.146.2", "8042"};
//   resolv.async_resolve(q, resolve_handler);
//   ioservice.run();
// }