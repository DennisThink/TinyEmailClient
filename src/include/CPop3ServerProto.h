#ifndef _CPP_EMAIL_POP3_SERVER_PROTO_CMD_H_
#define _CPP_EMAIL_POP3_SERVER_PROTO_CMD_H_
#include "CEmailProtoInterface.h"
#include "CProtoCode.h"
#include "user_info.h"
#include "CMemDataBase.h"
#include "thirdLib.h"
namespace cpp_email
{
    class CPop3ServerProto:public CEmailProtoInterface
    {
    public:
        explicit CPop3ServerProto(log_ptr_t log,CDataBaseInterface_SHARED_PTR ptr){
            m_dataBase = ptr;
            m_log=log;
            m_step = POP3_STEP_t::POP3_STEP_SERVER_ON_CONNECT;}
        virtual bool OnRecv(const std::string& strRecv) override;
        virtual std::string GetSend() override;
        virtual bool IsFinish() override { return false;}
    private:
        //POP3_STEP_t GetNextState(POP3_STEP_t cur,POP3_STEP_t recv);
        bool OnUser(const std::string strUser);
        bool OnPassword(const std::string strPasswd);
        std::string GetPassWordOkSend();
        std::string GetStatSend();
        POP3_STEP_t m_step;
        user_info m_UserInfo;
        CDataBaseInterface_SHARED_PTR m_dataBase=nullptr;
        log_ptr_t m_log;
        EmailInfoArray_t m_EmailArray;
    };
}
#endif
