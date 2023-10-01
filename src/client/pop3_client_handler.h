#ifndef _TINY_EMAIL_POP3_CLIENT_H_
#define _TINY_EMAIL_POP3_CLIENT_H_
#include <string>
#include <vector>
#include "CProtoCode.h"
#include "CProtoCmd.h"
namespace tiny_email
{
    struct Pop3EmailElem_t
    {
        std::string m_strIndex;
        std::size_t m_emailSize;
    };
    class CPop3ClientHandler
    {
    public:
        CPop3ClientHandler(const std::string strUserName, const std::string strPassword);
        std::string GetPop3Addr();
        bool FinishOrFailed();
        void OnReceive(const std::string strValue);
        bool IsServerRspCompleted(const std::string strRsp);
        std::string GetSend();

    private:
        void HandleListAllRsp(const std::string strRsp);
        void HandleGetOneUnread(const std::string strValue);
        void UpdateCurEmail();
        void HandleServerRsp(const std::string strRsp);
        std::string GetNextSend(const POP3_CLIENT_STEP_t curStep);
        POP3_CLIENT_STEP_t GetNextCmd(const POP3_CLIENT_STEP_t curStep, const Pop3Code_t code);
        std::vector<CSmtpProtoCmd> m_recvCmdVec;
        POP3_CLIENT_STEP_t m_step;

        bool m_bAuthFinish;
        std::string m_strEmailAddr;
        std::string m_strPop3Addr;
        std::string m_strUserName;
        std::string m_strPassword;
        std::string m_strReceiverAddr;
        std::string m_strContent;
        std::string m_strSend;
        std::string m_strRecv;
        std::string m_strListAll;
        std::vector<Pop3EmailElem_t> m_unreadIndexVec;
        std::string m_strEmailFormat;
        std::string m_strCurMailIndex;
        std::size_t m_nCurEmailCount;
        std::string m_strCurMailBegin;
    };
}
#endif
