cmake_minimum_required(VERSION 3.0)
project(tinyEmailClient)
set(CMAKE_CXX_STANDARD 11)
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
message("Windows")
add_definitions(-DWINDOWS)
add_definitions(-DWINDOWS_OS)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON)
else()
add_definitions(-DLINUX_OS)
SET(CMAKE_CXX_FLAGS "-g")
endif()

#add_definitions(-DOPENSSL)
include_directories(./include/thirdparty/socket-cpp/Socket/)
include_directories(./include/thirdparty/clipp/include/)
include_directories(./include/)
SET(NET_FILE 
     ./include/thirdparty/socket-cpp/Socket/TCPClient.cpp
     ./include/thirdparty/socket-cpp/Socket/TCPSSLClient.cpp 
     ./include/thirdparty/socket-cpp/Socket/SecureSocket.cpp
     ./include/thirdparty/socket-cpp/Socket/Socket.cpp)
SET(POP3_FILE ./client/pop3_client_handler.cpp ./include/ProtoUtil.cpp ./client/recv_mail.cpp ./include/CPop3ProtoCmd.cpp ./include/CProtoCmd.cpp)
SET(SMTP_FILE ./client/smtp_client_handler.cpp ./client/send_mail.cpp ./include/ProtoUtil.cpp ./include/CProtoCmd.cpp)
SET(IMAP_FILE ./client/imap_client_handler.cpp ./include/ProtoUtil.cpp ./include/CProtoCmd.cpp ./client/recv_mail.cpp)
add_executable(smtp ./client/smtp_main.cpp     ${NET_FILE} ${SMTP_FILE})
add_executable(pop3 ./client/pop3_main.cpp ${NET_FILE} ${POP3_FILE} ${IMAP_FILE})
add_executable(imap ./client/imap_main.cpp ${NET_FILE} ${IMAP_FILE} ${POP3_FILE})