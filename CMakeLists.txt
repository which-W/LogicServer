cmake_minimum_required(VERSION 3.15)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")  # /MTd
project("LogicServer" VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
#这是linux的
# 设置 Boost,jsoncpp 的路径
#set(BOOST_ROOT /usr)
#set(JSONCPP_INC_DIR /usr/local/include)

#这是给window写的
set(BOOST_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/Boost/x86/)
set(Boost_INCLUDE_DIR ${BOOST_ROOT}/include/boost.1.8/)
set(Boost_LIBRARY_DIR ${BOOST_ROOT}/lib)
set(Boost_NO_SYSTEM_PATHS ON)
#find_package(Boost REQUIRED COMPONENTS thread)

set(JSONCPP_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/include)
set(JSONCPP_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/lib)
file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/static/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable("LogicServer" ${SOURCES})

target_include_directories("LogicServer" PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/static
    ${BOOST_ROOT}/include/boost-1_8/
    ${JSONCPP_INC_DIR}
)

target_link_libraries("LogicServer" PRIVATE ${Boost_LIBRARY_DIR} ${JSONCPP_LIBRARY_DIR}/json_vc71_libmtd.lib)
