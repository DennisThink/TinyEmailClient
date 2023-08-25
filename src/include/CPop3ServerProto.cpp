#include "CPop3ServerProto.h"
#include "CPop3ProtoCmd.h"
namespace cpp_email
{

    std::string CPop3ServerProto::GetStatSend()
    {
        if(m_dataBase)
        {
            m_EmailArray.clear();
            m_dataBase->GetRecvMailInfo(m_UserInfo.userName_,m_EmailArray);
            std::size_t msgCound = m_EmailArray.size();
            std::size_t msgBytesCount = 0;
            for(const auto& item:m_EmailArray)
            {
                msgBytesCount += item.emailBytes_;
            }
            char buff[128]={0};
#ifdef WIN32
			sprintf_s(buff, "+OK %lu %lu \r\n", static_cast<unsigned long>(msgCound), static_cast<unsigned long>(msgBytesCount));
#else
            sprintf(buff,"+OK %lu %lu \r\n",static_cast<unsigned long>(msgCound),static_cast<unsigned long>(msgBytesCount));
#endif
			return buff;
        }
        else
        {
            return "+OK 0 0\r\n";
        }
    }


    std::string CPop3ServerProto::GetPassWordOkSend()
    {
        if(m_dataBase)
        {
            m_EmailArray.clear();
            m_log->info("USER:{}",m_UserInfo.userName_);
            m_dataBase->GetRecvMailInfo(m_UserInfo.userName_,m_EmailArray);
            std::size_t msgCound = m_EmailArray.size();
            std::size_t msgBytesCount = 0;
            for(const auto& item:m_EmailArray)
            {
                msgBytesCount += item.emailBytes_;
            }
            char buff[128]={0};
#ifdef WIN32
			sprintf_s(buff, "+OK %lu message(s) [%lu byte(s)]\r\n", static_cast<unsigned long>(msgCound), static_cast<unsigned long>(msgBytesCount));
#else
			sprintf(buff, "+OK %lu message(s) [%lu byte(s)]\r\n", static_cast<unsigned long>(msgCound), static_cast<unsigned long>(msgBytesCount));
#endif

            return buff;
        }
        else
        {
            return "+OK 0 message(s) [0 byte(s)]\r\n";
        }
    }
std::string CPop3ServerProto::GetSend()
{
    switch (m_step)
    {
    case POP3_STEP_t::POP3_STEP_SERVER_ON_CONNECT:
    {
        return "+OK Welcome to coremail Mail Pop3 Server (163coms[b62aaa251425b4be4eaec4ab4744cf47s])\r\n";
    }
    break;
    case POP3_STEP_t::POP3_STEP_SERVER_SEND_USER_NAME_OK:
    {
        return "+OK core mail user name\r\n";
    }
    break;
    case POP3_STEP_t::POP3_STEP_SERVER_SEND_USER_NAME_BAD:
    {
        return "-ERR core mail user name(cpp email)\r\n";
    }
    break;
    case POP3_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK:
    {
       return GetPassWordOkSend();
    }
    break;
    case POP3_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_BAD:
    {
        return "-ERR core mail pass word(cpp email)\r\n";
    }
    break;
    case POP3_STEP_t::POP3_STEP_SERVER_SEND_STAT_OK:
    {
        return "+OK 8 88108\r\n";
    }
    break;
    case POP3_STEP_t::POP3_STEP_SERVER_SEND_UIDL_OK:
    {
        return "+OK core mail pass word\r\n";
    }
    break;
    case POP3_STEP_t::POP3_STEP_SERVER_SEND_RETR_OK:
    {
        return "+OK core mail pass word\r\n";
    }
    break;
    case POP3_STEP_t::POP3_STEP_SERVER_SEND_NOOP_OK:
    {
        return "+OK core mail no op\r\n";
    }break;
    case POP3_STEP_t::POP3_STEP_SERVER_SEND_DELE_OK:
    {
        return "+OK core mail dele\r\n";
    }break;
    default:
    {
        return "";
    }
    break;
    }
}

bool CPop3ServerProto::OnUser(const std::string strUser)
{
    if(m_dataBase->IsUserExist(strUser))
    {
        m_step = POP3_STEP_t::POP3_STEP_SERVER_SEND_USER_NAME_OK;
        m_UserInfo.userName_ = strUser;
        return true;
    }
    else
    {
        m_step = POP3_STEP_t::POP3_STEP_SERVER_SEND_USER_NAME_BAD;
        return false;
    }
}

bool CPop3ServerProto::OnPassword(const std::string strPasswd)
{
    if(m_dataBase->IsPasswordRight(m_UserInfo.userName_,strPasswd))
    {
        m_step = POP3_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK;
        m_UserInfo.userPassword_ = strPasswd;
        return true;
    }
    else
    {
        m_step = POP3_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_BAD;
        return false;
    }
}

bool CPop3ServerProto::OnRecv(const std::string &strRecv)
{
    if (strRecv.empty())
    {
        return false;
    }
    CPop3ProtoReqCmd cmd;
    PARSE_POP3_RESULT result = CPop3ProtoReqCmd::FromString(strRecv, cmd);
    if (PARSE_POP3_RESULT::PARSE_POP3_SUCCEED == result)
    {
        switch (cmd.GetCode())
        {
        case POP3_CMD_t::POP3_CMD_USER_NAME:
        {
            return OnUser(cmd.GetMessage());
        }
        break;
        case POP3_CMD_t::POP3_CMD_PASS_WORD:
        {
            return OnPassword(cmd.GetMessage());
        }
        break;
        case POP3_CMD_t::POP3_CMD_STAT:
        {
            m_step = POP3_STEP_t::POP3_STEP_SERVER_SEND_STAT_OK;
        }
        break;
        case POP3_CMD_t::POP3_CMD_UIDL:
        {
            m_step = POP3_STEP_t::POP3_STEP_SERVER_SEND_UIDL_OK;
        }
        break;
        case POP3_CMD_t::POP3_CMD_RETR:
        {
            m_step = POP3_STEP_t::POP3_STEP_SERVER_SEND_RETR_OK;
        }
        break;
        case POP3_CMD_t::POP3_CMD_NOOP:
        {
            m_step = POP3_STEP_t::POP3_STEP_SERVER_SEND_NOOP_OK;
        }
        break;
        case POP3_CMD_t::POP3_CMD_DELE:
        {
            m_step = POP3_STEP_t::POP3_STEP_SERVER_SEND_DELE_OK;
        }
        break;
        default:
        {
        }
        break;
        }
        return true;
    }
    else
    {
        return false;
    }
    /*CPop3ProtoCmd curCmd = CPop3ProtoCmd::FromString(strRecv);
        if(curCmd.GetCode() == POP3_OK)
        {
            m_step = GetNextState(m_step,curCmd.GetCode());
        }*/
}

} // namespace cpp_email