#include "recv_mail.h"
#include "ssl_tcp_client.h"
#include "TCPClient.h"
#include "pop3_client_handler.h"
#include "ProtoUtil.h"

void Pop3RecvEmailSSL(const std::string strUserName, std::string strPassword, bool bDebug, std::string strPort)
{

    {
        tiny_email::CPop3ClientHandler handler(strUserName, strPassword);
        tiny_net::CSSLTcpClient client;
        tiny_net::CSSLTcpClient::Init();
        //std::string strSmtpPort = std::atoi(strPort.c_str());
        std::string strServerIp;
        int port = handler.GetServerSSLport();
        std::string strSmtpIp;
        client.Connect(strServerIp, port);
        char buff[128] = { 0 };

        std::string strServerRsp;
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