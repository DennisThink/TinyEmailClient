#include "recv_mail.h"
#include "TCPClient.h"
#include "imap_client_handler.h"
#include "ProtoUtil.h"
void ImapRecvEmail(const std::string strUserName, std::string strPassword, const std::string strServerAddr, std::string strPort, bool bDebug)
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
            tiny_email::CImapClientHandler handler(strUserName, strPassword);
            std::string strImapServer;
            int nImapPort;
            {
                if (strServerAddr.empty())
                {
                    std::string strTemp = handler.GetServerAddr();
                    strImapServer = tiny_email::CProtoUtil::AddrToIp(strTemp);
                }
                else
                {
                    strImapServer = tiny_email::CProtoUtil::AddrToIp(strServerAddr);
                }

                if (strPort.empty())
                {
                    nImapPort = handler.GetServerPort();
                }
                else
                {
                    nImapPort = std::atoi(strPort.c_str());
                }
            }
            std::cout << "IMAP SSL " << strImapServer << ":" << nImapPort << std::endl;
            char buff[128] = {0};
            std::string strServerRsp;
            CTCPClient tcpFd([](const std::string& strLogMsg)
                { std::cout << strLogMsg << std::endl; });
            if (!tcpFd.Connect(strImapServer, std::to_string(nImapPort)))
            {
                std::cout << " Connect Imap:  " << strImapServer << "  IP:  " << nImapPort << "  Failed  " << std::endl;
                return;
            }
            else
            {
                std::cout << "Imap:  " << strImapServer << "  IP:  " << nImapPort << std::endl;
                std::cout << "Connect Succeed" << std::endl;
            }
            while (!handler.IsFinished())
            {
                memset(buff, 0, 128);
                int nRecv = tcpFd.Receive(buff, 128, false);
                if (nRecv > 0)
                {
                    strServerRsp += std::string(buff);
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