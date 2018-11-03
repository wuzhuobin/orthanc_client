#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>

// std
#include <iostream>
// #include <string>
boost::asio::io_service io_service;
void connect_handler(const boost::system::error_code& error)
{
  if (error)
  {
    std::cerr << "Error: " << error.category().name() << " " << error.value() << '\n';
  }

}

void resolve_handler(const boost::system::error_code &error, boost::asio::ip::tcp::resolver::results_type results)
{
  if (error)
  {
    std::cerr << "Error: " << error.category().name() << " " << error.value() << '\n';
  }
  boost::asio::ip::tcp::socket socket(io_service);
  socket.async_connect(*results, connect_handler);
}

int main(int argc, char **argv)
{
  // boost::asio::ip::tcp::resolver()
  boost::asio::ip::tcp::resolver resolver(io_service);
  boost::asio::ip::tcp::socket socket(io_service);

  resolver.async_resolve("223.255.146.2", "8042", resolve_handler);
  io_service.run();
  std::cin.get();
  return 1;
}