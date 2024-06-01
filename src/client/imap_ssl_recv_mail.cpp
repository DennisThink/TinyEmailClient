#include "recv_mail.h"
#include "TCPClient.h"
#include "imap_client_handler.h"
#include "ProtoUtil.h"
#include "ssl_tcp_client.h"
void ImapRecvEmailSSL(const std::string strUserName, std::string strPassword, std::string strServer, std::string strPort, bool bDebug)
{

    {
        tiny_email::CImapClientHandler handler(strUserName, strPassword);
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