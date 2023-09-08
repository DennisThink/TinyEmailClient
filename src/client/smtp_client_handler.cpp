#include "smtp_client_handler.h"
#include "ProtoUtil.h"
#include <iostream>
namespace tiny_email{

 struct elem_t
    {
        elem_t(Smtp_Step_t curStep,
            ProtoCode_t protoCode,
            Smtp_Step_t nextStep)
        {
            curStep_ = curStep;
            protoCode_ = protoCode;
            nextStep_ = nextStep;
        }

        Smtp_Step_t curStep_;
        ProtoCode_t protoCode_;
        Smtp_Step_t nextStep_;
    };

    const static elem_t Array[] =
    {
        {SMTP_STEP_BEGIN, CODE_220, SEND_HELO},
        {SEND_HELO, CODE_250, SEND_AUTH_LOGIN},
        {SEND_AUTH_LOGIN, CODE_334, SEND_USER_NAME},
        {SEND_USER_NAME, CODE_334, SEND_PASSWORD},
        {SEND_PASSWORD, CODE_235, AUTH_SUCCESS},
        {SEND_PASSWORD,CODE_ANY,SMTP_FAILED},
        {AUTH_SUCCESS,CODE_ANY,SEND_MAIL_FROM},
        {SEND_MAIL_FROM,CODE_250,SEND_MAIL_TO},
        {SEND_MAIL_TO,CODE_250,SEND_DATA_HEAD},
        {SEND_DATA_HEAD,CODE_354,SEND_DATA_BODY},
        {SEND_DATA_BODY,CODE_354,SEND_DATA_TAIL},
        {SEND_DATA_TAIL,CODE_250,SEND_FINISH} };
    const int ARRAY_SIZE = sizeof(Array) / sizeof(Array[0]);
    void CSmtpClientHandler::OnReceive(const std::string strValue)
    {
        std::vector<std::string> strArray = CProtoUtil::SplitStringByLine(strValue);
        for (const auto& item : strArray)
        {
            if (item.length() > 3)
            {
                m_recvCmdVec.emplace_back(CSmtpProtoCmd::FromString(item));
            }
        }
        for (const auto& item : m_recvCmdVec)
        {
            if (item.isFinish())
            {
                m_step = GetNextCmd(m_step, item.GetCode());
                std::string strNext = GetNextSend(m_step);
                m_strSend += strNext;
                if (m_step == AUTH_SUCCESS)
                {
                    m_bAuthFinish = true;
                    m_step = GetNextCmd(m_step,CODE_ANY);
                    std::string strNext = GetNextSend(m_step);
                    m_strSend += strNext;
                }
            }
        }
        m_recvCmdVec.clear();
    }
    std::string CSmtpClientHandler::GetSmtpAddr()
    {
        return m_strSmtpAddr;
    }
    CSmtpClientHandler::CSmtpClientHandler(const std::string strUserName, const std::string strPassword)
    {
        m_strEmailAddr = strUserName;
        std::size_t index = strUserName.find_first_of("@");
        m_strUserName = strUserName.substr(0, index);
        m_strUserName = m_strUserName;
        m_strSmtpAddr = "smtp." + strUserName.substr(index + 1);
        m_strPassword = strPassword;
        m_step = SMTP_STEP_BEGIN;
    }
    bool CSmtpClientHandler::FinishOrFailed()
    {
        if(m_step == SEND_FINISH || m_step == SMTP_FAILED)
        {
            return true;
        }
        return false;
    }
    bool CSmtpClientHandler::SendMail(const std::string strReciver,const std::string strCarbonCopy, const std::string strMailContent,const std::string strSubject)
    {
        m_strReceiverAddr = strReciver;
        m_strCarbonCopy = strCarbonCopy;
        m_strContent = strMailContent;
        m_strSubject = strSubject;
        return true;
    }

    std::string CSmtpClientHandler::GetSend()
    {
        std::string strResult = m_strSend;
        m_strSend.clear();
        return strResult;
    }
    Smtp_Step_t CSmtpClientHandler::GetNextCmd(const Smtp_Step_t curStep, const ProtoCode_t code)
    {
        for (int i = 0; i < ARRAY_SIZE; i++)
        {
            if (Array[i].curStep_ == curStep &&
                (Array[i].protoCode_ == code || Array[i].protoCode_ == CODE_ANY))
            {
                return Array[i].nextStep_;
            }
        }
        return SEND_FINISH;
    }


    std::string CSmtpClientHandler::GetNextSend(const Smtp_Step_t curStep)
    {
        switch (curStep)
        {
        case SEND_HELO:
        {
            return "HELO " + m_strSmtpAddr+"\r\n";
        }
        break;
        case SEND_AUTH_LOGIN:
        {
            return "AUTH LOGIN\r\n";
        }
        break;
        case SEND_USER_NAME:
        {
            return CProtoUtil::Base64Encode(m_strUserName) + "\r\n";
        }
        break;
        case SEND_PASSWORD:
        {
            return CProtoUtil::Base64Encode(m_strPassword) + "\r\n";
        }break;
        case SEND_MAIL_FROM:
        {
            return "MAIL FROM: <" + m_strEmailAddr + ">\r\n";
        }break;
        case SEND_MAIL_TO:
        {
            return "RCPT TO: <"+ m_strReceiverAddr +">\r\n";
        }break;
        case SEND_DATA_HEAD:
        {
            return "DATA\r\n";
        }break;
        case SEND_DATA_BODY:
        {
            std::string strResult;
            {
                strResult += "Subject:Design Plan\r\n";
                strResult += "From: <";
                strResult += m_strEmailAddr;
                strResult += ">\r\n";
                strResult += "To: <"+ m_strReceiverAddr +">\r\n";
                strResult += "Cc: "+m_strCarbonCopy+"\r\n";
                strResult += "Subject: "+m_strSubject+"\r\n";
                strResult += "\r\n";
                strResult += m_strContent+"\r\n";
                strResult += "\r\n.\r\n";
            }
            return strResult;
        }break;
        case SEND_DATA_TAIL:
        {
            m_step = SEND_FINISH;
            return "\r\n.\r\n";
        }break;
        default:
        {
            return "";
        }
        break;
        }
    }
}
