find_package(Boost REQUIRED COMPONENTS system date_time regex filesystem)
list(
    APPEND
    PROJECT_LIBRARY
    ${Boost_LIBRARIES}
)
list(
    APPEND
    PROJECT_INCLUDE
    ${Boost_INCLUDE_DIRS}
)