#ifndef _SSL_TCP_CLIENT_H_
#define _SSL_TCP_CLIENT_H_
#include <string>

#ifdef WINDOWS_OS
#include <WinSock2.h>
#endif

#ifdef LINUX_OS
#define SOCKET int
#endif

namespace tiny_net
{
	class CSSLTcpClient
	{
	public:
		static void Init();
		static void Clear();


		void Connect(const std::string strIp, const int port);
		bool IsConnect() const;
		int Send(const char* pData, const std::size_t dataLen);
		int Recv(char* pData, const std::size_t dataLen);
		void CloseConnect();
	private:
		SOCKET m_sock;
		bool m_bConnected;
	};
}
#endif