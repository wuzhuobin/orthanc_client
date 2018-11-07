#ifndef __ORTHANC_CLIENT_H__
#define __ORTHANC_CLIENT_H__
#pragma once
#include <string>
#include <memory>
// #include <boost/smart_ptr/shared_ptr.hpp>
namespace boost{
namespace asio{
    class io_context;
}
}
struct orthanc_client_private;
class orthanc_client
{
public:
  orthanc_client(
      const std::string &server,
      const std::string &port);
    void method_GET(const std::string &path);
private:

    std::string server;
    std::string port;
    std::string path;
    std::shared_ptr<boost::asio::io_context> io_context;
    std::shared_ptr<orthanc_client_private> this_p;
    friend struct orthanc_client_private;
};

#endif // !__ORTHANC_CLIENT_H__