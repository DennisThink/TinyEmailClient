
#include "TCPClient.h"
#include "smtp_client_handler.h"
#include "ProtoUtil.h"
static bool g_bDebugOn = false;
void SendEmail(const std::string strUserName, std::string strPassword, std::string strReceiver, std::string strCarbonCopy, std::string Content, std::string strSubject, const bool bDebug, const std::string strSmtpServer, const std::string strPort)
{
    g_bDebugOn = bDebug;
    tiny_email::CSmtpClientHandler handler(strUserName, strPassword);
    CTCPClient tcpFd([](const std::string &strLogMsg)
    { if(g_bDebugOn){
    std::cout << strLogMsg << std::endl; 
    } });
    std::string strSmtpPort = strPort;
    std::string strSmtpIp = "";
    if (strSmtpPort.empty())
    {
        strSmtpPort = std::to_string(handler.GetServerPort());
    }

    if (strSmtpServer.empty())
    {
        std::string strSmtpAddr = handler.GetServerAddr();
        strSmtpIp = tiny_email::CProtoUtil::AddrToIp(strSmtpAddr);
    }
    if (bDebug)
    {
        std::cout << "SMTP Addr: " << strSmtpIp<<":"<<strSmtpPort << std::endl;
    }
    if (!tcpFd.Connect(strSmtpIp, strSmtpPort))
    {
        return;
    }
    char buff[128] = {0};

    std::string strServerRsp;
    handler.SendMail(strReceiver, strCarbonCopy, Content, strSubject);
    while (!handler.IsFinished())
    {
        memset(buff, 0, 128);
        int nRecv = tcpFd.Receive(buff, 128, false);
        if (nRecv > 0)
        {
            std::string strValue(buff, nRecv);
            strServerRsp += strValue;
            if (handler.IsServerRspCompleted(strServerRsp))
            {
                handler.OnServerCommand(strServerRsp);
                std::string strClientReq = handler.GetResponse();
                if (!strClientReq.empty())
                {
                    if (bDebug)
                    {
                        std::cout << "S: " << strServerRsp << std::endl;
                        std::cout << "C: " << strClientReq << std::endl;
                    }
                    strServerRsp.clear();
                    tcpFd.Send(strClientReq);
                }
            }          
        }
    }
}