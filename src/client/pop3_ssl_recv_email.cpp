#include "recv_mail.h"
#include "ssl_tcp_client.h"
#include "TCPClient.h"
#include "pop3_client_handler.h"
#include "ProtoUtil.h"

void Pop3RecvEmailSSL(const std::string strUserName, std::string strPassword,std::string strServerAddr,std::string strPort, bool bDebug)
{
    {
        tiny_email::CPop3ClientHandler handler(strUserName, strPassword);
        tiny_net::CSSLTcpClient client;
        tiny_net::CSSLTcpClient::Init();
        std::string strPop3SSLServer;
        int nPop3SSLPort;
        {
            if (strServerAddr.empty())
            {
                std::string strTemp = handler.GetServerAddr();
                strPop3SSLServer = tiny_email::CProtoUtil::AddrToIp(strTemp);
            }
            else
            {
                strPop3SSLServer = tiny_email::CProtoUtil::AddrToIp(strServerAddr);
            }

            if (strPort.empty())
            {
                nPop3SSLPort = handler.GetServerSSLport();
            }
            else
            {
                nPop3SSLPort = std::atoi(strPort.c_str());
            }
        }
        std::cout << "POP3 SSL  " << strPop3SSLServer << ":" << nPop3SSLPort << std::endl;
        client.Connect(strPop3SSLServer, nPop3SSLPort);
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