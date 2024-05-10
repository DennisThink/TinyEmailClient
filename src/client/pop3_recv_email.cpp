#include "recv_mail.h"
#include "TCPClient.h"
#include "pop3_client_handler.h"
#include "ProtoUtil.h"

void Pop3RecvEmail(const std::string strUserName, std::string strPassword, bool bDebug, std::string strPort)
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
            std::string strPop3Addr = handler.GetServerAddr();
            std::string strPop3Ip = tiny_email::CProtoUtil::AddrToIp(strPop3Addr);
            std::string strPop3Port = strPort;
            std::cout << "Pop3:  " << strPop3Addr << "  IP:  " << strPop3Ip << std::endl;
            CTCPClient tcpFd([](const std::string &strLogMsg)
                             { std::cout << strLogMsg << std::endl; });
            if (!tcpFd.Connect(strPop3Ip, strPop3Port))
            {
                return;
            }
            else
            {
                std::cout << "Pop3:  " << strPop3Addr << "  IP:  " << strPop3Ip << std::endl;
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