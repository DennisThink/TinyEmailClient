#include "send_mail.h"
#include "ssl_tcp_client.h"
#include "smtp_client_handler.h"
#include "ProtoUtil.h"
#include <iostream>
void SendEmailSSL(const std::string strUserName, std::string strPassword, std::string strReceiver, std::string strCarbonCopy, std::string Content, std::string strSubject, const bool bDebug, const std::string strSmtpServer, const std::string strPort)
{
    tiny_email::CSmtpClientHandler handler(strUserName, strPassword);
    tiny_net::CSSLTcpClient client;
    tiny_net::CSSLTcpClient::Init();
    {
        std::string strSSlServerIp;
        int nSSLport=0;
        if (strSmtpServer.empty())
        {
            std::string strSmtpAddr = handler.GetServerAddr();
            strSSlServerIp = tiny_email::CProtoUtil::AddrToIp(strSmtpAddr);
        }
        else
        {
            strSSlServerIp = tiny_email::CProtoUtil::AddrToIp(strSmtpServer);
        }

        if (strPort.empty())
        {
            nSSLport = handler.GetServerSSLport();
        }
        else
        {
            nSSLport = std::atoi(strPort.c_str());
        }
        if (bDebug)
        {
            std::cout << "SMTP Addr: " << strSSlServerIp << ":" << nSSLport << std::endl;
        }
        client.Connect(strSSlServerIp,nSSLport);
        char buff[128] = { 0 };

        std::string strServerRsp;
        handler.SendMail(strReceiver, strCarbonCopy, Content, strSubject);
        while (!handler.IsFinished())
        {
            memset(buff, 0, 128);
            int nRecv = client.Recv(buff, 128);
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
                        client.Send(strClientReq.c_str(), strClientReq.length());
                    }
                }
            }
        }
        tiny_net::CSSLTcpClient::Clear();
    }
}