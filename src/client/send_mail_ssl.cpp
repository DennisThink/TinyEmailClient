#include "TCPSSLClient.h"
#include "smtp_client_handler.h"
#include "ProtoUtil.h"
static bool g_bDebugOn = false;
void SendEmailSSL(const std::string strUserName, std::string strPassword, std::string strReceiver, std::string strCarbonCopy, std::string Content, std::string strSubject, const bool bDebug, const std::string strSmtpServer, const std::string strPort)
{
    g_bDebugOn = bDebug;
    tiny_email::CSmtpClientHandler handler(strUserName, strPassword);
    CTCPSSLClient tcpFd([](const std::string &strLogMsg)
                        {
        if (g_bDebugOn)
        {
            std::cout << strLogMsg << std::endl;
        } });
    std::string strSmtpPort = strPort;
    std::string strSmtpIp = "";
    if (strSmtpPort.empty())
    {
        strSmtpPort = "465";
    }

    if (strSmtpServer.empty())
    {
        std::string strSmtpAddr = handler.GetSmtpAddr();
        strSmtpIp = tiny_email::CProtoUtil::AddrToIp(strSmtpAddr);
    }
    if (bDebug)
    {
        std::cout << "SMTP Addr: " << strSmtpIp << "  Port: " << strSmtpPort << std::endl;
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