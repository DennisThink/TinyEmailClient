#include "recv_mail.h"
#include "TCPClient.h"
#include "imap_client_handler.h"
#include "ProtoUtil.h"
#include "ssl_tcp_client.h"
void ImapRecvEmailSSL(const std::string strUserName, std::string strPassword, std::string strServerAddr, std::string strPort, bool bDebug)
{

    {
        tiny_email::CImapClientHandler handler(strUserName, strPassword);
        tiny_net::CSSLTcpClient client;
        tiny_net::CSSLTcpClient::Init();
        std::string strImapSSLServer;
        int nImapSSLPort;
        {
            if (strServerAddr.empty())
            {
                std::string strTemp = handler.GetServerAddr();
                strImapSSLServer = tiny_email::CProtoUtil::AddrToIp(strTemp);
            }
            else
            {
                strImapSSLServer = tiny_email::CProtoUtil::AddrToIp(strServerAddr);
            }

            if (strPort.empty())
            {
                nImapSSLPort = handler.GetServerSSLport();
            }
            else
            {
                nImapSSLPort = std::atoi(strPort.c_str());
            }
        }
        std::cout << "IMAP SSL " << strImapSSLServer << ":" << nImapSSLPort << std::endl;
        client.Connect(strImapSSLServer, nImapSSLPort);
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