cmake_minimum_required(VERSION 3.15)

include(cmake/HunterGate.cmake)

HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.240.tar.gz"
        SHA1 "ca19f3769e6c80cfdd19d8b12ba5102c27b074e0"
)

project(BeastIssue1818)

hunter_add_package(Boost COMPONENTS filesystem system)
hunter_add_package(OpenSSL)
hunter_add_package(Catch)

find_package(Boost CONFIG COMPONENTS filesystem system)
find_package(OpenSSL)
find_package(Threads)
find_package(Catch2)

file(GLOB_RECURSE src_files CONFIGURE_DEPENDS "src/*.cpp" "src/*.hpp")
add_executable(check ${src_files})
target_include_directories(check PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(check PRIVATE Catch2::Catch2 Boost::filesystem Boost::system OpenSSL::Crypto OpenSSL::SSL Threads::Threads)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(check PRIVATE -Werror -Wall -Wextra -pedantic)
endif()