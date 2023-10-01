#include "TCPSSLClient.h"
#include "smtp_client_handler.h"
namespace SMTL_SSL{
    static bool g_bDebugOn = false;
    auto LogPrinterSSL = [](const std::string &strLogMsg)
    {
        if (g_bDebugOn)
        {
            std::cout << strLogMsg << std::endl;
        }
    };

    static std::string GetSmtpServerIpAddr(const std::string strSmtpAddr)
    {
        struct hostent *hent = nullptr;
        struct in_addr **addr_list = nullptr;
        int i;
        if ((hent = gethostbyname(strSmtpAddr.c_str())) == NULL)
        {
            // herror("gethostbyname error");
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

    void SendEmailSSL(const std::string strUserName, std::string strPassword, std::string strReceiver, std::string strCarbonCopy, std::string Content, std::string strSubject, const bool bDebug, const std::string strSmtpServer, const std::string strPort)
    {
        g_bDebugOn = bDebug;
        tiny_email::CSmtpClientHandler handler(strUserName, strPassword);
        CTCPSSLClient tcpFd(LogPrinterSSL);
        std::string strSmtpPort = strPort;
        std::string strSmtpIp = "";
        if (strSmtpPort.empty())
        {
            strSmtpPort = "25";
        }

        if (strSmtpServer.empty())
        {
            std::string strSmtpAddr = handler.GetSmtpAddr();
            strSmtpIp = GetSmtpServerIpAddr(strSmtpAddr);
        }
        if (bDebug)
        {
            std::cout << "SMTP Addr: " << strSmtpIp << std::endl;
        }
        if (!tcpFd.Connect(strSmtpIp, strSmtpPort))
        {
            return;
        }
        char buff[128] = {0};

        std::string strLog;
        handler.SendMail(strReceiver, strCarbonCopy, Content, strSubject);
        while (!handler.FinishOrFailed())
        {
            memset(buff, 0, 128);
            int nRecv = tcpFd.Receive(buff, 128, false);
            std::string strValue(buff, nRecv);
            strLog += strValue;
            handler.OnReceive(strValue);
            std::string strMsg = handler.GetSend();
            if (!strMsg.empty())
            {
                if (bDebug)
                {
                    std::cout << "S: " << strLog << std::endl;
                    strLog.clear();
                    std::cout << "C: " << strMsg << std::endl;
                }
                tcpFd.Send(strMsg);
            }
        }
    }
};