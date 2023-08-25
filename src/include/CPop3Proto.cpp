#include "CPop3Proto.h"
#include "CPop3ProtoCmd.h"
namespace cpp_email
{
    std::string CPop3Proto::GetSend()
    {
        switch(m_step)
        {
            case POP3_SEND_USER_NAME:
            {
                return m_reqVec[0]->GetString();
            }break;
            case POP3_SEND_PASS_WORD:
            {
                return m_reqVec[1]->GetString();
            }break;
            case POP3_SHOW_FIRST_EMAIL:
            {
                return "LIST 1\r\n";
            }break;
            case POP3_RECEIVE_FIRST_EMAIL:
            {
                return "RETR 1\r\n";
            }break;
            case POP3_QUIT:
            {
                return "QUIT\r\n";
            }break;
            default:
            {
                return "";
            }
        }
        return "";
    }

    struct POP3Elem
    {
        POP3Elem(Pop3Code_t cur,Pop3Code_t recv,Pop3Code_t next)
        {
            curState_ = cur;
            recvState_ = recv;
            nextState_ = next;
        }
        Pop3Code_t curState_;
        Pop3Code_t recvState_;
        Pop3Code_t nextState_;
    };
    POP3Elem Array[]=
    {
        {POP3_VERIFY                , POP3_OK, POP3_SEND_USER_NAME},
        {POP3_SEND_USER_NAME        , POP3_OK, POP3_SEND_PASS_WORD},
        {POP3_SEND_PASS_WORD        , POP3_OK, POP3_SHOW_FIRST_EMAIL},
        {POP3_SHOW_FIRST_EMAIL      , POP3_OK, POP3_RECEIVE_FIRST_EMAIL},
        {POP3_RECEIVE_FIRST_EMAIL   , POP3_OK, POP3_QUIT},
    };
    int ARRAY_SIZE = sizeof(Array)/sizeof(Array[0]);

    Pop3Code_t CPop3Proto::GetNextState(Pop3Code_t cur,Pop3Code_t recv)
    {
        for(int i = 0 ; i < ARRAY_SIZE ; i++)
        {
            if(Array[i].curState_ == cur &&
               Array[i].recvState_ == recv)
            {
                return Array[i].nextState_;
            }
        }
        return POP3_NO_OP;
    }
    bool CPop3Proto::OnParseMessage(const std::string& strRecv)
    {
 switch(m_step)
        {
            case POP3_SEND_USER_NAME:
            {
                return OnUserNameRsp(strRecv);
            }break;
            case POP3_SEND_PASS_WORD:
            {
                return OnPasswordRsp(strRecv);
            }break;
            case POP3_SHOW_FIRST_EMAIL:
            {
                return true;
            }break;
            case POP3_RECEIVE_FIRST_EMAIL:
            {
                return true;
            }break;
            case POP3_QUIT:
            {
                return true;
            }break;
            default:
            {
                return true;
            }
        }
    }
    bool CPop3Proto::OnRecv(const std::string& strRecv)
    {
        bool result = OnParseMessage(strRecv);
        if(result)
        {
            m_step = GetNextState(m_step,POP3_OK);
        }
        else
        {
            m_step = GetNextState(m_step,POP3_BAD);
        }
         
        return true;
    }

    bool CPop3Proto::OnUserNameRsp(const std::string& strRecv)
    {
        CPop3UserNameRsp rsp;
        if(rsp.FromString(strRecv))
        {
            if(rsp.GetResult() == PARSE_POP3_RESULT::PARSE_POP3_SUCCEED)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    bool CPop3Proto::OnPasswordRsp(const std::string& strRecv)
    {
        CPop3UserNameRsp rsp;
        if(rsp.FromString(strRecv))
        {
            if(rsp.GetResult() == PARSE_POP3_RESULT::PARSE_POP3_SUCCEED)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
}