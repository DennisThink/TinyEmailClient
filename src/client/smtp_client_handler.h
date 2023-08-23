#ifndef _TINY_EMAIL_SMTP_CLIENT_HANDLER_H_
#define _TINY_EMAIL_SMTP_CLIENT_HANDLER_H_
#include <string>
#include <vector>
#include "CProtoCode.h"
#include "CProtoCmd.h"
namespace tiny_email{
class CSmtpClientHandler
    {
    public:
        CSmtpClientHandler(const std::string strUserName, const std::string strPassword);
        std::string GetSmtpAddr();
        bool SendMail(const std::string strReciver, const std::string strMailContent);
        bool FinishOrFailed();
        void OnReceive(const std::string strValue);
        std::string GetSend();
    private:
        std::string GetNextSend(const Smtp_Step_t curStep);
        Smtp_Step_t GetNextCmd(const Smtp_Step_t curStep, const ProtoCode_t code);
        std::vector<CSmtpProtoCmd> m_recvCmdVec;
        Smtp_Step_t m_step;
        bool m_bAuthFinish;
        std::string m_strEmailAddr;
        std::string m_strSmtpAddr;
        std::string m_strUserName;
        std::string m_strPassword;
        std::string m_strReceiverAddr;
        std::string m_strContent;
        std::string m_strSend;
    };
}
#endif