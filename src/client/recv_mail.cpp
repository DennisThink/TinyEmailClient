#include "recv_mail.h"
#include "TCPClient.h"
#include "pop3_client_handler.h"
auto LogPrinter2 = [](const std::string &strLogMsg)
{ std::cout << strLogMsg << std::endl; };

std::string GetPop3ServerIpAddr(const std::string strSmtpAddr)
{
    struct hostent *hent = nullptr;
    struct in_addr **addr_list = nullptr;
    int i;
    if ((hent = gethostbyname(strSmtpAddr.c_str())) == NULL)
    {
        herror("gethostbyname error");
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
void RecvEmail(const std::string strUserName, std::string strPassword)
{
    tiny_email::CPop3ClientHandler handler(strUserName,strPassword);
    std::string strPop3Addr = handler.GetPop3Addr();
    std::string strPop3Ip = GetPop3ServerIpAddr(strPop3Addr);
    std::string strPop3Port = "110";

    CTCPClient tcpFd(LogPrinter2);
    tcpFd.Connect(strPop3Ip, strPop3Port);
    char buff[1024] = {0};

    while (!handler.FinishOrFailed())
    {
        memset(buff, 0, 1024);
        tcpFd.Receive(buff, 1024, false);
        std::string strValue(buff);
        std::cout<<"S: "<<strValue<<std::endl;
        handler.OnReceive(strValue);
        std::string strMsg = handler.GetSend();
        if (!strMsg.empty())
        {
            std::cout<<"C:  "<<strMsg<<std::endl;
            tcpFd.Send(strMsg);
        }
    }

}