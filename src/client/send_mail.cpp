
#include "TCPClient.h"
#include "smtp_client_handler.h"
auto LogPrinter = [](const std::string &strLogMsg)
{ std::cout << strLogMsg << std::endl; };

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
void SendEmail(const std::string strUserName, std::string strPassword, std::string strReceiver, std::string Content)
{
    tiny_email::CSmtpClientHandler handler(strUserName, strPassword);
    CTCPClient tcpFd(LogPrinter);
    std::string strPort = "25";
    std::string strIp = GetSmtpServerIpAddr(handler.GetSmtpAddr());
    tcpFd.Connect(strIp, strPort);
    char buff[128] = {0};

    handler.SendMail(strReceiver, Content);
    while (!handler.FinishOrFailed())
    {
        memset(buff, 0, 128);
        tcpFd.Receive(buff, 128, false);
        std::string strValue(buff);
        handler.OnReceive(strValue);
        std::string strMsg = handler.GetSend();
        if (!strMsg.empty())
        {
            tcpFd.Send(strMsg);
        }
    }
}