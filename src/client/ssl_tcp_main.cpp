#include "ssl_tcp_client.h"
#include "TCPClient.h"
#include "smtp_client_handler.h"
#include "ProtoUtil.h"
#include <iostream>
//#include "clipp.h"
int SSL_TEST(int argc, char* argv[])
{
	using namespace tiny_net;
	CSSLTcpClient client;
	CSSLTcpClient::Init();
	client.Connect("127.0.0.1", 4433);
	while (client.IsConnect())
	{
		{
			char sendBuff[32] = { 0 };
			strcpy(sendBuff, "helloworld\r\n");
			if (client.Send(sendBuff, strlen(sendBuff)) > 0)
			{

			}
			else
			{
				break;
			}
		}
		{
			char recvBuff[32]={0};
			if (client.Recv(recvBuff, 32))
			{

			}
			else
			{
				break;
			}
		}
	}
	client.CloseConnect();
	CSSLTcpClient::Clear();
	return 0;
}

static bool g_bDebugOn = false;
void SendEmail_SSL(const std::string strUserName, std::string strPassword, std::string strReceiver, std::string strCarbonCopy, std::string Content, std::string strSubject, const bool bDebug, const std::string strSmtpServer, const std::string strPort)
{
    g_bDebugOn = bDebug;
    tiny_email::CSmtpClientHandler handler(strUserName, strPassword);
    tiny_net::CSSLTcpClient client;
    tiny_net::CSSLTcpClient::Init();
    //std::string strSmtpPort = std::atoi(strPort.c_str());
    int port = handler.GetServerSSLport();
    std::string strSmtpIp;
    if (strSmtpServer.empty())
    {
        std::string strSmtpAddr = handler.GetServerAddr();
        //strSmtpIp = tiny_email::CProtoUtil::AddrToIp(strSmtpAddr);
    }
    if (bDebug)
    {
        std::cout << "SMTP Addr: " << strSmtpIp << ":" << port << std::endl;
    }
    client.Connect(strSmtpServer, port);
    char buff[128] = { 0 };

    std::string strServerRsp;
    handler.SendMail(strReceiver, strCarbonCopy, Content, strSubject);
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
}

int SMTP_SSL_SEND(int argc, char* *argv)
{
    {
        //using namespace clipp;
        std::string strUser;
        std::string strPassword;
        std::string strReciver;
        std::string strCarbonCopy;
        std::string strBlindCarbonCopy;
        std::string strContent;
        std::string strSubject;
        std::string strSmtpServer;
        std::string strPort;
        bool bSetCarbonCopy = false;
        bool bSetBlindCarbonCopy = false;
        bool bSetEmailSubject = false;
        bool bDebugOn = false;
        bool bSetSmtpServer = false;
        bool bSetPort = false;
        /*auto cli = (
            required("-u", "--user").doc("the account you used for send email") & value("user", strUser),
            required("-p", "--password").doc("the password of the account") & value("password", strPassword),
            required("-r", "--receiver").doc("email address to receive email") & value("receiver", strReciver),
            option("-cc", "--carboncopy").doc("email address to copy email to").set(bSetCarbonCopy) & value("carboncopy", strCarbonCopy),
            option("-bcc", "--blind-carbon-copy").doc("email address to copy email to but secrect").set(bSetBlindCarbonCopy) & value("blind carboncopy", strBlindCarbonCopy),
            option("-s", "--subject").doc("email subject").set(bSetEmailSubject) & value("email subject", strSubject),
            option("-d", "--debug").doc("enable debug mode").set(bDebugOn),
            required("-t", "--context").doc("email content") & value("context", strContent),
            option("-server", "--server").doc("smtp server addr").set(bSetSmtpServer) & value("smtp server ip addr", strSmtpServer),
            option("-port", "--port").doc("smtp server port").set(bSetPort) & value("email subject", strPort));
        */
        //if (!parse(argc, argv, cli))
        {
            //std::cout << make_man_page(cli, argv[0]) << std::endl;
          //  return 0;
        }
        //else
        {
            SendEmail_SSL(strUser, strPassword, strReciver, strCarbonCopy, strContent, strSubject, bDebugOn, strSmtpServer, strPort);
        }
        //SendEmail_SSL(strUser, strPassword, strReciver, strCarbonCopy, strContent, strSubject, bDebugOn, strSmtpServer, strPort);
    }

    return 0;
}

int main(int argc, char** argv)
{
    return SMTP_SSL_SEND(argc, argv);
}