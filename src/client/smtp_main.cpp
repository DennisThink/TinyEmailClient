
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
#include "send_mail.h"
#include "recv_mail.h"
#include "clipp.h"
int main(int argc, char *argv[])
{
    {
        using namespace clipp;
        std::string strUser;
        std::string strPassword;
        std::string strReciver;
        std::string strCarbonCopy;
        std::string strBlindCarbonCopy;
        std::string strContent;
        std::string strSubject;
        std::string strSmtpServer;
        std::string strPort;
        bool bSetCarbonCopy=false;
        bool bSetBlindCarbonCopy=false;
        bool bSetEmailSubject=false;
        bool bDebugOn=false;
        bool bSetSmtpServer = false;
        bool bSetPort = false;
        auto cli = (
                    required("-u", "--user").doc("the account you used for send email")&value("user",strUser),
                    required("-p", "--password").doc("the password of the account")&value("password",strPassword),
                    required("-r", "--receiver").doc("email address to receive email")&value("receiver",strReciver),
                    option("-cc","--carboncopy").doc("email address to copy email to").set(bSetCarbonCopy)&value("carboncopy",strCarbonCopy),
                    option("-bcc","--blind-carbon-copy").doc("email address to copy email to but secrect").set(bSetBlindCarbonCopy)&value("blind carboncopy",strBlindCarbonCopy),
                    option("-s","--subject").doc("email subject").set(bSetEmailSubject)&value("email subject",strSubject),
                    option("-d","--debug").doc("enable debug mode").set(bDebugOn),
                    required("-t", "--context").doc("email content")&value("context",strContent),
                    option("-server","--server").doc("smtp server addr").set(bSetSmtpServer)&value("smtp server ip addr",strSmtpServer),
                    option("-port","--port").doc("smtp server port").set(bSetPort)&value("email subject",strPort));

        if (!parse(argc, argv, cli))
        {
            std::cout << make_man_page(cli, argv[0])<<std::endl;
            return 0;
        }
        else
        {
            SendEmail(strUser, strPassword, strReciver,strCarbonCopy, strContent,strSubject,bDebugOn,strSmtpServer,strPort);
        }
    }
    return 0;
}
