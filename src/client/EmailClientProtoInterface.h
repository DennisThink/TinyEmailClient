#ifndef _TINY_EMAIL_EMAIL_CLIENT_PROTO_INTERFAE_H_
#define _TINY_EMAIL_EMAIL_CLIENT_PROTO_INTERFAE_H_
#include <string>
#include <string.h>
namespace tiny_email
{
    class EmailClientProtoInterface
    {
    public:
        EmailClientProtoInterface(const std::string strEmailAddr, const std::string strPassword);
        virtual std::string GetServerAddr();
        virtual int GetServerPort();
        virtual int GetServerSSLport();
        virtual bool IsServerRspCompleted(const std::string strRsp);
        virtual void OnServerCommand(const std::string strMsg);
        virtual bool IsFinished();
        std::string GetResponse();
    protected:

    protected:
        std::string m_strEmailAddr;
        std::string m_strPassword;
        std::string m_strServerAddr;
        std::string m_strResponse;
    };
};
#endif