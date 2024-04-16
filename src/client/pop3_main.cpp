
/**
 * @file main.cpp
 * @author DennisThink (dennisthink@hotmail.com)
 * @brief this is a simple program to send email
 * @version 0.1
 * @date 2023-09-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include "recv_mail.h"
#include "clipp.h"
int main(int argc, char *argv[])
{
    {
        using namespace clipp;
        std::string strUser;
        std::string strPassword;
        std::string strPort="110";
        bool bDebugOn=false;
        bool bSetPort = false;
        auto cli = (
                    required("-u", "--user").doc("the account you used for send email")&value("user",strUser),
                    required("-p", "--password").doc("the password of the account")&value("password",strPassword),
                    option("-d","--debug").doc("enable debug mode").set(bDebugOn),
                    option("-pp","--port").doc("pop3 server port").set(bSetPort)&opt_value("port",strPort));
        if (!parse(argc, argv, cli))
        {
            std::cout << make_man_page(cli, argv[0])<<std::endl;
            return 0;
        }
        else
        {
            Pop3RecvEmail(strUser, strPassword,bDebugOn,strPort);
        }
    }
    return 0;
}
