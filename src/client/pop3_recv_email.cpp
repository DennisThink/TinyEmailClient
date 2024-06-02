#include "recv_mail.h"
#include "TCPClient.h"
#include "pop3_client_handler.h"
#include "ProtoUtil.h"

void Pop3RecvEmail(const std::string strUserName, std::string strPassword,std::string strServerAddr,std::string strPort, bool bDebug)
{
    {
#ifdef WINDOWS_OS
        WORD wVersionRequested;
        WSADATA wsaData;
        int err;
        wVersionRequested = MAKEWORD(1, 1);
        err = WSAStartup(wVersionRequested, &wsaData);
        if (err != 0)
        {
            /* Tell the user that we couldn't find a useable */
            /* winsock.dll. */
            return;
        }
#endif

        {
            tiny_email::CPop3ClientHandler handler(strUserName, strPassword);
            std::string strPop3Server;
            std::string strPop3Port;
            {
                if (strServerAddr.empty())
                {
                    std::string strAddr = handler.GetServerAddr();
                    strPop3Server = tiny_email::CProtoUtil::AddrToIp(strAddr);
                }
                else
                {
                    strPop3Server = tiny_email::CProtoUtil::AddrToIp(strServerAddr);
                }
                
                if (strPort.empty())
                {
                    strPop3Port = std::to_string(handler.GetServerPort());
                }
                else
                {
                    strPop3Port = strPort;
                }
              
                std::string strPop3Port = strPort;
               
            }
           
            CTCPClient tcpFd([](const std::string &strLogMsg)
                             { std::cout << strLogMsg << std::endl; });
            if (!tcpFd.Connect(strPop3Server, strPop3Port))
            {
                std::cout << " Connect Pop3:  " << strPop3Server << "  IP:  " << strPop3Port <<"  Failed  "<< std::endl;
                return;
            }
            else
            {
                std::cout << "Pop3:  " << strPop3Server << "  IP:  " << strPop3Port << std::endl;
                std::cout << "Connect Succeed" << std::endl;
            }
            char buff[128] = {0};

            std::string strRsp;
            while (!handler.IsFinished())
            {
                memset(buff, 0, 128);
                int ret = tcpFd.Receive(buff, 127, false);
                if (ret > 0)
                {
                    strRsp += std::string(buff);
                    if (handler.IsServerRspCompleted(strRsp))
                    {
                        handler.OnServerCommand(strRsp);
                        std::string strMsg = handler.GetResponse();
                        if (!strMsg.empty())
                        {
                            tcpFd.Send(strMsg);
                        }
                        if (bDebug)
                        {
                            std::cout << "S: " << strRsp << std::endl;
                            std::cout << "C: " << strMsg << std::endl;
                        }
                        strRsp.clear();
                    }
                }
              
            }
        }

#ifdef WINDOWS_OS
        /* Confirm that the Windows Sockets DLL supports 1.1.*/
        /* Note that if the DLL supports versions greater */
        /* than 1.1 in addition to 1.1, it will still return */
        /* 1.1 in wVersion since that is the version we */
        /* requested. */
        if (LOBYTE(wsaData.wVersion) != 1 ||
            HIBYTE(wsaData.wVersion) != 1)
        {
            /* Tell the user that we couldn't find a useable */
            /* winsock.dll. */
            WSACleanup();
            return;
        }
#endif
    }
}