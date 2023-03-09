cmake_minimum_required(VERSION 3.21)
project(threadPool)


set(CMAKE_CXX_STANDARD 14)

# Set the path to the MySQL Connector/C library installation directory
set(MYSQLCONNECTORCPP_DIR /usr/local/mysql-8.0.32-macos13-x86_64/lib)

# Find the MySQL Connector/C library
find_library(MYSQLCONNECTORCPP_LIBRARY mysqlclient HINTS ${MYSQLCONNECTORCPP_DIR})

# Add the MySQL Connector/C headers to the include directories
include_directories(/usr/local/mysql-8.0.32-macos13-x86_64/include)

# Add your application source files here
add_executable(threadPool main.cpp Connection.cpp CommonConnectionPool.cpp)

# Link against the MySQL Connector/C library
target_link_libraries(threadPool ${MYSQLCONNECTORCPP_LIBRARY})
