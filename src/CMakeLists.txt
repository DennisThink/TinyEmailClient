cmake_minimum_required(VERSION 3.0)
project(tinyEmailClient)
set(CMAKE_CXX_STANDARD 11)
include_directories(./include/thirdparty/socket-cpp/Socket/)
include_directories(./include/)
SET(NET_FILE 
     ./include/thirdparty/socket-cpp/Socket/TCPClient.cpp 
     ./include/thirdparty/socket-cpp/Socket/Socket.cpp)
SET(SMTP_FILE ./client/smtp_client_handler.cpp ./client/send_mail.cpp ./include/ProtoUtil.cpp ./include/CProtoCmd.cpp)
add_executable(client ./client/main.cpp     ${NET_FILE} ${SMTP_FILE})