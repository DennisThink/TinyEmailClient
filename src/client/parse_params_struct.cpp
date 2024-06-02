#include "params_struct.h"
#include "clipp.h"
#include <iostream>
SendEmailParam_t ParseSendParam(int argc, char* argv[])
{
    SendEmailParam_t result;
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
        bool bSetCarbonCopy = false;
        bool bSetBlindCarbonCopy = false;
        bool bSetEmailSubject = false;
        bool bDebugOn = false;
        bool bSetSmtpServer = false;
        bool bSetPort = false;
        auto cli = (
            required("-u", "--user").doc("the account you used for send email") & value("user", strUser),
            required("-p", "--password").doc("the password of the account") & value("password", strPassword),
            required("-r", "--receiver").doc("email address to receive email") & value("receiver", strReciver),
            option("-cc", "--carboncopy").doc("email address to copy email to").set(bSetCarbonCopy) & value("carboncopy", strCarbonCopy),
            option("-bcc", "--blind-carbon-copy").doc("email address to copy email to but secrect").set(bSetBlindCarbonCopy) & value("blind carboncopy", strBlindCarbonCopy),
            option("-s", "--subject").doc("email subject").set(bSetEmailSubject) & value("email subject", strSubject),
            option("-d", "--debug").doc("enable debug mode").set(bDebugOn),
            required("-t", "--context").doc("email content") & value("context", strContent),
            option("-server", "--server").doc("smtp server addr").set(bSetSmtpServer) & value("smtp server ip addr", strSmtpServer),
            option("-port", "--port").doc("smtp server port").set(bSetPort) & value("email subject", strPort));
       
        if (!parse(argc, argv, cli))
        {
            std::cout << make_man_page(cli, argv[0]) << std::endl;
            result._bParseSucceed = false;
            return result;
        }
        else
        {
            result._bDebug = bDebugOn;
            result._strUserAddress = strUser;
            result._strUserPassword = strPassword;
            result._strServerAddr = strSmtpServer;
            result._strServerPort = strPort;
            result._strRecvAddr = strReciver;
            result._strCopyReceiver = strCarbonCopy;
            result._strSubject = strSubject;
            result._strContext = strContent;
            result._bParseSucceed = true;
            return result;
        }
    }
}

RecvEmailParams_t ParseRecvParam(int argc, char* argv[])
{
    RecvEmailParams_t result;
    {
        using namespace clipp;
        std::string strUser;
        std::string strPassword;
        std::string strServer;
        std::string strPort;
        bool bDebugOn = false;
        bool bSetPort = false;
        auto cli = (
            required("-u", "--user").doc("the account you used for send email") & value("user", strUser),
            required("-p", "--password").doc("the password of the account") & value("password", strPassword),
            option("-s", "--server").doc("imap server addr").set(strServer) & opt_value("server", strServer),
            option("-pp", "--port").doc("imap server port").set(bSetPort) & opt_value("port", strPort),
            option("-d", "--debug").doc("enable debug mode").set(bDebugOn));

        if (!parse(argc, argv, cli))
        {
            std::cout << make_man_page(cli, argv[0]) << std::endl;
            result._bParseSucceed = false;
            return result;
        }
        else
        {
            result._bDebug = bDebugOn;
            result._strUserAddress = strUser;
            result._strUserPassword = strPassword;
            result._strServerAddr = strServer;
            result._strServerPort = strPort;
            result._bParseSucceed = true;
            return result;
        }
    }
}