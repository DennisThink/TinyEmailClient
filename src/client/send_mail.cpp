
#include "TCPClient.h"
#include "smtp_client_handler.h"
static bool g_bDebugOn=false;
auto LogPrinter = [](const std::string &strLogMsg)
{ if(g_bDebugOn){
    std::cout << strLogMsg << std::endl; 
    }
};

std::string GetSmtpServerIpAddr(const std::string strSmtpAddr)
{
    struct hostent *hent = nullptr;
    struct in_addr **addr_list = nullptr;
    int i;
    if ((hent = gethostbyname(strSmtpAddr.c_str())) == NULL)
    {
        //herror("gethostbyname error");
        return "";
    }
    addr_list = (struct in_addr **)hent->h_addr_list;
    char ip[100] = {0};
    for (int i = 0; addr_list[i] != NULL; i++)
    {
        strcpy(ip, inet_ntoa(*addr_list[i]));
        return ip;
    }
    return "";
}

void SendEmail(const std::string strUserName, std::string strPassword, std::string strReceiver,std::string strCarbonCopy, std::string Content,std::string strSubject,const bool bDebug)
{
    g_bDebugOn = bDebug;
    tiny_email::CSmtpClientHandler handler(strUserName, strPassword);
    CTCPClient tcpFd(LogPrinter);
    std::string strPort = "25";
    std::string strIp = GetSmtpServerIpAddr(handler.GetSmtpAddr());
    tcpFd.Connect(strIp, strPort);
    char buff[128] = {0};

    std::string strLog;
    handler.SendMail(strReceiver,strCarbonCopy, Content,strSubject);
    while (!handler.FinishOrFailed())
    {
        memset(buff, 0, 128);
        tcpFd.Receive(buff, 128, false);
        std::string strValue(buff);
        strLog+=strValue;
        handler.OnReceive(strValue);
        std::string strMsg = handler.GetSend();
        if (!strMsg.empty())
        {
            if(bDebug)
            {
                std::cout<<"S: "<<strLog<<std::endl;
                std::cout<<"C: "<<strMsg<<std::endl;
            }
            tcpFd.Send(strMsg);
        }
    }
}