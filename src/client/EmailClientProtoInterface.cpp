#include "EmailClientProtoInterface.h"
namespace tiny_email
{
    EmailClientProtoInterface::EmailClientProtoInterface(const std::string strEmailAddr,const std::string strPassword)
    {
        m_strEmailAddr = strEmailAddr;
        m_strPassword = strPassword;
    }
    
    std::string EmailClientProtoInterface::GetServerAddr()
    {
        return "";
    }
    
    int EmailClientProtoInterface::GetServerPort()
    {
        return -1;
    }
    
    int EmailClientProtoInterface::GetServerSSLport()
    {
        return -1;
    }
    
    void EmailClientProtoInterface::OnReceive(const std::string strValue)
    {

    }
    
    bool EmailClientProtoInterface::IsServerRspCompleted(const std::string strRsp)
    {
        return false;
    }

    std::string EmailClientProtoInterface::GetResponse()
    {
        std::string result = m_strResponse;
        m_strResponse.clear();
        return result;
    }

    bool EmailClientProtoInterface::IsFinished()
    {
        return false;
    }
}