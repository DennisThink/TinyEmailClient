#ifndef _TINY_EMAIL_POP3_CLIENT_H_
#define _TINY_EMAIL_POP3_CLIENT_H_
#include <string>
#include <vector>
#include "CProtoCode.h"
#include "CProtoCmd.h"
#include "EmailClientProtoInterface.h"
#include "email_struct.h"
namespace tiny_email
{
    class CPop3ClientHandler:public EmailClientProtoInterface
    {
    public:
        CPop3ClientHandler(const std::string strUserName, const std::string strPassword);
        virtual std::string GetServerAddr() override;
        virtual int GetServerPort() override;
        virtual int GetServerSSLport() override;
        virtual bool IsServerRspCompleted(const std::string strRsp) override;
        virtual void OnServerCommand(const std::string strRsp) override;
        virtual bool IsFinished() override;
    private:
        void HandleListAllRsp(const std::string strRsp);
        void HandleGetOneUnread(const std::string strValue);
        void UpdateCurEmail();
        void HandleServerRsp(const std::string strRsp);
        POP3_CLIENT_STEP_t GetNextCmd(const POP3_CLIENT_STEP_t curStep, const Pop3Code_t code);
        std::string CmdToRequestContext(const POP3_CLIENT_STEP_t curStep);
        std::vector<CSmtpProtoCmd> m_recvCmdVec;
        POP3_CLIENT_STEP_t m_step;

        bool m_bAuthFinish;
        std::string m_strReceiverAddr;
        std::string m_strContent;
        std::string m_strRecv;
        std::string m_strListAll;
        std::vector<EmailElement_t> m_unreadIndexVec;
        std::string m_strEmailFormat;
        std::string m_strUserName;
        std::string m_strCurMailIndex;
        std::size_t m_nCurEmailCount;
        std::string m_strCurMailBegin;
    };
}
#endif
