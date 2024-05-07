#ifndef _TINY_EMAIL_IMAP_CLIENT_H_
#define _TINY_EMAIL_IMAP_CLIENT_H_
#include <string>
#include <vector>
#include "CProtoCode.h"
#include "CProtoCmd.h"
#include "EmailClientProtoInterface.h"
#include "email_struct.h"
namespace tiny_email
{
    class CImapClientHandler:public EmailClientProtoInterface
    {
    public:
        CImapClientHandler(const std::string strUserName, const std::string strPassword);

        
        bool IsServerRspCompleted(const std::string strRsp);
        virtual void OnReceive(const std::string strValue) override;
        virtual std::string GetServerAddr() override;
        virtual int GetServerPort() override;
        virtual int GetServerSSLport() override;
    private:
        void HandleListAllRsp(const std::string strRsp);
        void HandleGetOneUnread(const std::string strValue);
        void UpdateCurEmail();
        void HandleServerRsp(const std::string strRsp);
        std::string GetNextSend(const POP3_CLIENT_STEP_t curStep);
        POP3_CLIENT_STEP_t GetNextCmd(const POP3_CLIENT_STEP_t curStep, const Pop3Code_t code);
        std::vector<CSmtpProtoCmd> m_recvCmdVec;
        IMAP_CLIENT_STEP_T m_step;

        bool m_bAuthFinish;
        std::size_t m_nCommandIndex;
        std::string m_strEmailAddr;
        std::string m_strImapAddr;
        std::string m_strUserName;
        std::string m_strPassword;
        std::string m_strReceiverAddr;
        std::string m_strContent;
        std::string m_strSend;
        std::string m_strRecv;
        std::string m_strListAll;
        std::vector<EmailElement_t> m_unreadIndexVec;
        std::string m_strEmailFormat;
        std::string m_strCurMailIndex;
        std::size_t m_nCurEmailCount;
        std::string m_strCurMailBegin;
    };
}
#endif
