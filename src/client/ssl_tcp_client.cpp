#include "ssl_tcp_client.h"


#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#ifdef WINDOWS_OS
#include <WinSock2.h>
#endif

#ifdef LINUX_OS
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#define SOCKET int
#endif

namespace tiny_net
{
    SSL_CTX* g_sslCtx;
    SSL* g_ssl;
#ifdef WINDOWS_OS
    static WSADATA g_wsaData;
    extern "C" WSADATA WinSockInit()
    {
        WORD wVersionRequested;
        WSADATA wsaData;
        int err;
        wVersionRequested = MAKEWORD(1, 1);
        err = WSAStartup(wVersionRequested, &wsaData);
        if (err != 0)
        {
            /* Tell the user that we couldn't find a useable */
            /* winsock.dll. */
            return wsaData;
        }
        return wsaData;
    }

    extern "C" void WinSockClean(const WSADATA wsaData)
    {
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
    }
#endif


    extern "C" void InitSSLEnv()
    {
        // create server ssl context
        OpenSSL_add_all_algorithms(); // set cryptos
        SSL_load_error_strings(); // set error messages
        SSL_library_init(); // init ssl lib
        const SSL_METHOD* method = TLS_client_method(); // create client method
        g_sslCtx = SSL_CTX_new(method); // create client context
        if (g_sslCtx == NULL) ERR_print_errors_fp(stderr); // print error
        g_ssl = SSL_new(g_sslCtx); // hold data for the SSL cocnnection
    }

    void CSSLTcpClient::Init()
    {
#ifdef WINDOWS_OS
        g_wsaData = WinSockInit();
#endif
        InitSSLEnv();
    }

    void CSSLTcpClient::Clear()
    {
#ifdef WINDOWS_OS
        WinSockClean(g_wsaData);
#endif
    }


    void CSSLTcpClient::Connect(const std::string strIp, const int port)
    {
        struct hostent* host;
        struct sockaddr_in addr;
        m_sock = socket(PF_INET, SOCK_STREAM, 0);
#ifdef WINDOWS_OS
        if ((host = gethostbyname(strIp.c_str())) == NULL) exit(-1); // get host by name
#endif

#ifdef LINUX_OS

        if ((host = gethostbyname(strIp.c_str())) == NULL) exit(-1); // get host by name
#endif
        memset(&addr, sizeof(addr), 0); // memset address with 0
        addr.sin_family = AF_INET; // IPv4 address family
        addr.sin_port = htons(port); // convert to network short byte order
        addr.sin_addr.s_addr = *(long*)(host->h_addr);  // set the IP of the socket; sin_addr is an union
        if (connect(m_sock, (struct sockaddr*)&addr, sizeof(addr)) != 0)
        {
            m_bConnected = false;
        }
        else
        {
            m_bConnected = true;
            int ret = SSL_set_fd(g_ssl, m_sock); // assigns a socket to a SSL structure
            ret = SSL_connect(g_ssl);
        }
    }
    bool CSSLTcpClient::IsConnect() const
    {
        return m_bConnected;
    }

    int CSSLTcpClient::Send(const char* pData, const std::size_t dataLen)
    {
        int nRet =  SSL_write(g_ssl, pData, dataLen);
        if (nRet < 0)
        {
            m_bConnected = false;
        }
        return nRet;
    }
    int CSSLTcpClient::Recv(char* pData, const std::size_t dataLen)
    {
        int nRet =  SSL_read(g_ssl, pData, dataLen);
        if (nRet < 0)
        {
            m_bConnected = false;
        }
        return nRet;
    }
    void CSSLTcpClient::CloseConnect()
    {
        SSL_free(g_ssl); // close ssl
        SSL_CTX_free(g_sslCtx); // release context
        closesocket(m_sock);
    }
}