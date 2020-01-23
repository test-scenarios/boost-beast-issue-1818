#pragma once

#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>

namespace boost{ namespace filesystem {}}

namespace program
{
    namespace net = boost::asio;
    namespace beast = boost::beast;
    namespace ssl = boost::asio::ssl;
    namespace fs = boost::filesystem;
    namespace system = boost::system;
}