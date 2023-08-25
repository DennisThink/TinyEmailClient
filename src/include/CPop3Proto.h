#ifndef _CPP_EMAIL_POP3_PROTO_CMD_H_
#define _CPP_EMAIL_POP3_PROTO_CMD_H_
#include "CEmailProtoInterface.h"
#include "CProtoCode.h"
#include "CPop3ProtoCmd.h"
#include <vector>
namespace cpp_email
{
    class CPop3Proto:public CEmailProtoInterface
    {
    public:
        explicit CPop3Proto(std::string strUserName,std::string strPasswd){
            m_userName = strUserName;
            m_password = strPasswd;
            m_step = POP3_VERIFY;
            m_reqVec.push_back(std::make_shared<CPop3UserNameReq>(m_userName));
            m_reqVec.push_back(std::make_shared<CPop3PasswordReq>(m_password));
        }
        virtual bool OnRecv(const std::string& strRecv) override;
        virtual std::string GetSend() override;
        virtual bool IsFinish() override {return false;}
    private:
        bool OnParseMessage(const std::string& strRecv);
        bool OnUserNameRsp(const std::string& strRecv);
        bool OnPasswordRsp(const std::string& strRecv);
        Pop3Code_t GetNextState(Pop3Code_t cur,Pop3Code_t recv);
        Pop3Code_t m_step;
        std::string m_userName;
        std::string m_password;
        std::vector<CPop3ReqBase_PTR_T> m_reqVec;
    };
}
#endif
