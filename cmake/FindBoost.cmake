find_package(Boost REQUIRED COMPONENTS system date_time regex)
list(
    APPEND
    PROJECT_LIBRARY
    Boost::system 
    Boost::date_time
    Boost::regex
)