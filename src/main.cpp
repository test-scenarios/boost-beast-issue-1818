#include <iostream>
#include "explain.hpp"
#include "config.hpp"

#define CATCH_CONFIG_RUNNER

#include <catch2/catch.hpp>
#include <boost/filesystem.hpp>

namespace {
using namespace program;

struct temp_file
{
    temp_file(temp_file &&) = default;

    temp_file(temp_file const &) = delete;

    temp_file &
    operator=(temp_file &&) = default;

    temp_file
    operator=(temp_file const &) = delete;

    temp_file()
        : path_(fs::temp_directory_path() / fs::unique_path())
    {

    }

    ~temp_file()
    {
        if (!path_.empty())
        {
            system::error_code ec;
            fs::remove(path_, ec);
            boost::ignore_unused(ec);
        }
    }

    auto path() -> fs::path const&
    {
        return path_;
    }

private:

    fs::path path_;
};

TEST_CASE("file truncates")
{
    auto test_file = temp_file();

    auto header = beast::http::request_header<beast::http::fields>();
    header.target("/");
    header.version(11);
    header.method(beast::http::verb::get);
    header.set("Server", "test");
    header.set("Content-Length", "200000");

    auto body = beast::http::file_body::value_type();
    auto body_reader = beast::http::file_body::reader(header, body);
    auto error = beast::error_code();
    body.open(test_file.path().native().c_str(), beast::file_mode::write_new, error);
    CHECK(!error.value());
    body_reader.init(200000, error);

    for (int i = 0 ; i < 10000 ; ++i)
    {
        body_reader.put(net::buffer("0123456789", 10), error);
        REQUIRE(!error);
    }

    ::truncate(test_file.path().native().c_str(), 0);

    for (int i = 0 ; i < 10000 && !error ; ++i)
    {
        body_reader.put(net::buffer("0123456789", 10), error);
        CHECK(!error);
    }

    body_reader.finish(error);
    CHECK(!error);

    CHECK(body.size() == 200000);
    body.close();

    CHECK(fs::file_size(test_file.path()) == 200000);
}

}

int
main(
    int argc,
    char **argv)
{
    try
    {
        return Catch::Session().run(argc, argv);
    }
    catch (...)
    {
        std::cerr << program::explain() << std::endl;
        return 127;
    }
}