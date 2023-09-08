# TinyEmailClient

## 中文文档:

tinyEmailClient是一个简单的邮件发送程序，采用C++语言编写，cmake编译工具进行编译。目前支持Ubuntu 22.04和Windows 7系统。
代码的编写采用了数据通信和协议处理分离的方式，你可以采用代码的协议处理部分，而使用自己的通信处理代码。

### 代码编译:

1. tinyEmailClient使用cmake编译工具，请先安装cmake编译工具并设置好路径。
2. tinyEmailClient使用c++语言编写,请保证您的c++编译器支持c++11。
3. 在前两个条件准备好的基础上，就可以使用cmake工具进行编译了。

###  代码运行
1. 
SYNOPSIS
        ./client -u <user> -p <password> -r <receiver> -t <context> -s <subject>

OPTIONS
        -u, --user  the account you used for send email
        -p, --password
                    the password of the account

        -r, --receiver
                    email address to receive email

        -t, --context
                    email content

        -s, --subject
                    email subject


### 第三方库

本程序使用了clipp作为命令行参数解析，socket-cpp实现网络通信。

仓库地址:
1. clipp (https://github.com/muellan/clipp)
2. socket-cpp https://github.com/embeddedmz/socket-cpp

## English Document:

tinyEmailClient is a very simple email send program which is writen by cpp and compiled by cmake tool.tinyEmailClient support ubuntu 22.04 and windows 7 now.The smtp protocal process code and communication code is writen seperatly,so you can use the protocal process code and your own communication code.

### how to compile:
1. tinyEmailClient use cmake tool to compile,so please make sure you have installed the cmake,
2. tinyEmailClient was writen by  cpp with the standard on c++11,so please make sure your compiler support c++11.
3. if the above two condition is prepared, you can compile the project.

### how to run the program
1. use command line to run the program

SYNOPSIS
        ./client -u <user> -p <password> -r <receiver> -t <context> -s <subject>

OPTIONS
        -u, --user  the account you used for send email
        -p, --password
                    the password of the account

        -r, --receiver
                    email address to receive email

        -t, --context
                    email content

        -s, --subject
                    email subject


### third party library to use
1. this program use clipp to parse command arguments and socket-cpp for network.

Git Repositories:

1. clipp (https://github.com/muellan/clipp)
2. socket-cpp https://github.com/embeddedmz/socket-cpp