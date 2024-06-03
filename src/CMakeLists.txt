cmake_minimum_required(VERSION 3.0)

project(tinyEmailClient)
set(CMAKE_CXX_STANDARD 11)
#distinguish between different plantform
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
message("Windows")
add_definitions(-DWINDOWS)
add_definitions(-DWINDOWS_OS)
add_compile_definitions(WINDOWS_OS)
link_directories(./libs/)
else()
add_definitions(-DLINUX_OS)
SET(CMAKE_CXX_FLAGS "-g")
endif()


include_directories(./include/thirdparty/socket-cpp/Socket/)
include_directories(./include/thirdparty/clipp/include/)
include_directories(./include/thirdparty/ssl_inc/)
include_directories(./include/thirdparty/)
include_directories(./include/)
include_directories(./client/)
SET(NET_FILE 
     ./include/thirdparty/socket-cpp/Socket/TCPClient.cpp
     ./include/thirdparty/socket-cpp/Socket/TCPSSLClient.cpp 
     ./include/thirdparty/socket-cpp/Socket/SecureSocket.cpp
     ./include/thirdparty/socket-cpp/Socket/Socket.cpp)

SET(POP3_FILE ./client/pop3_client_handler.cpp
              ./include/ProtoUtil.cpp 
              ./client/pop3_recv_email.cpp 
              ./client/EmailClientProtoInterface.cpp 
              ./include/CPop3ProtoCmd.cpp 
              ./include/CProtoCmd.cpp)

SET(SMTP_FILE ./client/smtp_client_handler.cpp 
               ./client/send_mail.cpp  
               ./include/ProtoUtil.cpp 
               ./client/EmailClientProtoInterface.cpp 
               ./include/CProtoCmd.cpp)

SET(IMAP_FILE  ./client/imap_client_handler.cpp 
               ./include/ProtoUtil.cpp 
               ./include/CProtoCmd.cpp 
               ./client/EmailClientProtoInterface.cpp 
               ./client/imap_recv_mail.cpp)

add_executable(smtp ./client/smtp_main.cpp ./client/parse_params_struct.cpp ${NET_FILE} ${SMTP_FILE})
add_executable(pop3 ./client/pop3_main.cpp ./client/parse_params_struct.cpp ${NET_FILE} ${POP3_FILE})
add_executable(imap ./client/imap_main.cpp ./client/parse_params_struct.cpp ${NET_FILE} ${IMAP_FILE})
add_executable(ssl_smtp ./client/smtp_ssl_main.cpp
                ./client/send_mail_ssl.cpp
                ./client/ssl_tcp_client.cpp
                ./client/smtp_client_handler.cpp 
                ./include/ProtoUtil.cpp 
                ./client/EmailClientProtoInterface.cpp 
                ./include/CProtoCmd.cpp
                ./client/parse_params_struct.cpp)

add_executable(ssl_pop3 
                ./client/pop3_ssl_main.cpp 
                ./client/parse_params_struct.cpp
                ./client/pop3_ssl_recv_email.cpp
                ./client/ssl_tcp_client.cpp
                ${NET_FILE} ${POP3_FILE})

add_executable(ssl_imap 
                    ./client/imap_ssl_main.cpp 
                    ./client/parse_params_struct.cpp
                    ./client/imap_ssl_recv_mail.cpp
                    ./client/ssl_tcp_client.cpp
                    ${NET_FILE} ${IMAP_FILE})

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
target_link_libraries(ssl_smtp libssl-1_1-x64.lib)
target_link_libraries(ssl_smtp libcrypto-1_1-x64.lib)
target_link_libraries(ssl_smtp ws2_32.lib)

target_link_libraries(ssl_pop3 libssl-1_1-x64.lib)
target_link_libraries(ssl_pop3 libcrypto-1_1-x64.lib)
target_link_libraries(ssl_pop3 ws2_32.lib)

target_link_libraries(ssl_imap libssl-1_1-x64.lib)
target_link_libraries(ssl_imap libcrypto-1_1-x64.lib)
target_link_libraries(ssl_imap ws2_32.lib)
else()
target_link_libraries(ssl_smtp libssl.a)
target_link_libraries(ssl_smtp libcrypto.a)

target_link_libraries(ssl_pop3 libssl.a)
target_link_libraries(ssl_pop3 libcrypto.a)

target_link_libraries(ssl_imap libssl.a)
target_link_libraries(ssl_imap libcrypto.a)
endif()