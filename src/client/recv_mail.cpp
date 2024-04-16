#include "recv_mail.h"
#include "TCPClient.h"
#include "pop3_client_handler.h"
#include "imap_client_handler.h"
#include "ProtoUtil.h"
void ImapRecvEmail(const std::string strUserName, std::string strPassword, const std::string strIp, std::string strPort, bool bDebug)
{
    {
        WORD wVersionRequested;
        WSADATA wsaData;
        int err;
        wVersionRequested = MAKEWORD(1, 1);
        err = WSAStartup(wVersionRequested, &wsaData);
        if (err != 0) {
            /* Tell the user that we couldn't find a useable */
            /* winsock.dll. */
            return;
        }
        {
            tiny_email::CImapClientHandler handler(strUserName, strPassword);
            std::string strPop3Addr = handler.GetPop3Addr();
            std::string strPop3Ip = tiny_email::CProtoUtil::AddrToIp(strPop3Addr);
            std::string strPop3Port = strPort;
            std::cout << "IMAP:  " << strPop3Addr << "  IP:  " << strPop3Ip << std::endl;
            CTCPClient tcpFd([](const std::string& strLogMsg)
                { std::cout << strLogMsg << std::endl; });
            if (!tcpFd.Connect(strPop3Ip, strPop3Port))
            {
                return;
            }
            else
            {
                std::cout << "IMAP:  " << strPop3Addr << "  IP:  " << strPop3Ip << std::endl;
                std::cout << "Connect Succeed" << std::endl;
            }
            char buff[2048] = { 0 };

            while (!handler.FinishOrFailed())
            {
                memset(buff, 0, 2048);
                int ret = tcpFd.Receive(buff, 2048, false);
                if (ret <= 0)
                {
                    break;
                }
                std::string strValue(buff, ret);
                if (bDebug)
                {
                    std::cout << "S: " << strValue << std::endl;
                }
                handler.OnReceive(strValue);
                std::string strMsg = handler.GetSend();
                if (!strMsg.empty())
                {
                    std::cout << "C:  " << strMsg << std::endl;
                    tcpFd.Send(strMsg);
                }
            }
        }
        /* Confirm that the Windows Sockets DLL supports 1.1.*/
        /* Note that if the DLL supports versions greater */
        /* than 1.1 in addition to 1.1, it will still return */
        /* 1.1 in wVersion since that is the version we */
        /* requested. */
        if (LOBYTE(wsaData.wVersion) != 1 ||
            HIBYTE(wsaData.wVersion) != 1) {
            /* Tell the user that we couldn't find a useable */
            /* winsock.dll. */
            WSACleanup();
            return;
        }
    }
}
void Pop3RecvEmail(const std::string strUserName, std::string strPassword, bool bDebug, std::string strPort)
{
    {
        WORD wVersionRequested;
        WSADATA wsaData;
        int err;
        wVersionRequested = MAKEWORD(1, 1);
        err = WSAStartup(wVersionRequested, &wsaData);
        if (err != 0) {
            /* Tell the user that we couldn't find a useable */
            /* winsock.dll. */
            return;
        }
        {
            tiny_email::CPop3ClientHandler handler(strUserName, strPassword);
            std::string strPop3Addr = handler.GetPop3Addr();
            std::string strPop3Ip = tiny_email::CProtoUtil::AddrToIp(strPop3Addr);
            std::string strPop3Port = strPort;
            std::cout << "Pop3:  " << strPop3Addr << "  IP:  " << strPop3Ip << std::endl;
            CTCPClient tcpFd([](const std::string& strLogMsg)
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
            char buff[2048] = { 0 };

            while (!handler.FinishOrFailed())
            {
                memset(buff, 0, 2048);
                int ret = tcpFd.Receive(buff, 2048, false);
                if (ret <= 0)
                {
                    break;
                }
                std::string strValue(buff, ret);
                if (bDebug)
                {
                    std::cout << "S: " << strValue << std::endl;
                }
                handler.OnReceive(strValue);
                std::string strMsg = handler.GetSend();
                if (!strMsg.empty())
                {
                    std::cout << "C:  " << strMsg << std::endl;
                    tcpFd.Send(strMsg);
                }
            }
        }
        /* Confirm that the Windows Sockets DLL supports 1.1.*/
        /* Note that if the DLL supports versions greater */
        /* than 1.1 in addition to 1.1, it will still return */
        /* 1.1 in wVersion since that is the version we */
        /* requested. */
        if (LOBYTE(wsaData.wVersion) != 1 ||
            HIBYTE(wsaData.wVersion) != 1) {
            /* Tell the user that we couldn't find a useable */
            /* winsock.dll. */
            WSACleanup();
            return;
        }
    }
    
}