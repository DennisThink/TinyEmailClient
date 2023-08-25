#include "pop3_client_handler.h"
#include <iostream>
#include "ProtoUtil.h"
#include "CPop3ProtoCmd.h"
namespace tiny_email
{
        CPop3ClientHandler::CPop3ClientHandler(const std::string strUserName, const std::string strPassword)
        {
                m_strUserName = strUserName;
                m_strPassword = strPassword;
                std::size_t index = m_strUserName.find_first_of("@");
                m_strPop3Addr = "pop." + m_strUserName.substr(index + 1);
                m_strUserName = m_strUserName.substr(0, index);
                m_step = POP3_CLIENT_STEP_t::POP3_CLIENT_STEP_BEGIN;
        }
        std::string CPop3ClientHandler::GetPop3Addr()
        {
                return m_strPop3Addr;
        }
        bool CPop3ClientHandler::FinishOrFailed()
        {
                return false;
        }
        std::string CPop3ClientHandler::GetSend()
        {
                std::string strResult = m_strSend;
                m_strSend.clear();
                return strResult;
        }
        bool CPop3ClientHandler::IsServerRspCompleted(const std::string strRsp)
        {
                if (m_step == POP3_CLIENT_STEP_t::POP3_CLIENT_LIST_ALL)
                {
                        if (strRsp.find('.') != std::string::npos)
                        {
                                return true;
                        }
                        else
                        {
                                return false;
                        }
                }
                if (m_step == POP3_CLIENT_STEP_t::POP3_CLIENT_GET_ONE_UNREAD)
                {
                        m_strRecv += strRsp;
                        if (m_strRecv.length() + strRsp.length() < m_unreadIndexVec.begin()->m_emailSize)
                        {
                                return false;
                        }
                        else
                        {
                                return true;
                        }
                }
                if (strRsp.find("\r\n") != std::string::npos)
                {
                        return true;
                }
                return false;
        }

        void CPop3ClientHandler::HandleListAllRsp(const std::string strRsp)
        {
                std::vector<std::string> result = CProtoUtil::SplitStringByLine(strRsp);
                m_unreadIndexVec.clear();
                for (std::size_t i = 1; i < result.size() - 1; i++)
                {
                        std::size_t index = result[i].find_first_of(' ');
                        std::string strIndex = result[i].substr(0, index);
                        std::size_t count = std::atol(result[i].substr(index).c_str());
                        m_unreadIndexVec.push_back({strIndex, count});
                }
                {
                        std::cout << "----------------------List All-------------------------------" << std::endl;
                        for (auto item : m_unreadIndexVec)
                        {
                                std::cout << item.m_strIndex << "------" << item.m_emailSize << std::endl;
                        }
                        std::cout << "----------------------List All-------------------------------" << std::endl;
                }
                return;
        }
        void CPop3ClientHandler::HandleGetOneUnread(const std::string strValue)
        {
                m_strEmailFormat += strValue;
                std::cout << "----------------------------------------" << std::endl;
                std::cout << "EMail: " << m_strEmailFormat << std::endl;
                std::cout << "----------------------------------------" << std::endl;
                std::cout << "EMailLen: " << m_strEmailFormat.length() << "     " << m_strEmailCount << std::endl;
                m_strEmailFormat.clear();
                m_strEmailCount = 0;
                if (!m_unreadIndexVec.empty())
                {
                        std::string strCmd = "RETR " + (m_unreadIndexVec.begin()->m_strIndex) + "\r\n";
                        m_strEmailCount = m_unreadIndexVec.begin()->m_emailSize;
                        m_unreadIndexVec.erase(m_unreadIndexVec.begin());
                }
                else
                {
                        m_step = POP3_CLIENT_STEP_t::POP3_CLIENT_STEP_END;
                        std::string strCmd = "QUIT \r\n";
                }
        }
        void CPop3ClientHandler::HandleServerRsp(const std::string strValue)
        {
                if (m_step == POP3_CLIENT_STEP_t::POP3_CLIENT_LIST_ALL)
                {
                        HandleListAllRsp(strValue);
                        m_step = POP3_CLIENT_GET_ONE_UNREAD;
                        m_step = GetNextCmd(m_step, POP3_ANY);
                        m_strSend = GetNextSend(m_step);
                        return;
                }
                if (m_step == POP3_CLIENT_STEP_t::POP3_CLIENT_GET_ONE_UNREAD)
                {
                        HandleGetOneUnread(strValue);
                        m_step = GetNextCmd(m_step, POP3_ANY);
                        m_strSend = GetNextSend(m_step);
                        return;
                }
                CPop3ProtoCmd curCmd = CPop3ProtoCmd::FromString(strValue);
                m_step = GetNextCmd(m_step, curCmd.GetCode());
                m_strSend = GetNextSend(m_step);
        }

        void CPop3ClientHandler::OnReceive(const std::string strValue)
        {
                m_strRecv += strValue;
                if (IsServerRspCompleted(m_strRecv))
                {
                        std::cout << "-------------------Finished------" << std::endl;
                        std::string strRsp = m_strRecv;
                        HandleServerRsp(strRsp);
                        m_strRecv.clear();
                }
        }

        std::string CPop3ClientHandler::GetNextSend(const POP3_CLIENT_STEP_t curStep)
        {
                if (curStep == POP3_CLIENT_STEP_t::POP3_CLIENT_SEND_USER_NAME)
                {
                        return "USER " + m_strUserName + "\r\n";
                }

                if (curStep == POP3_CLIENT_STEP_t::POP3_CLIENT_SEND_PASS_WORD)
                {
                        return "PASS " + m_strPassword + "\r\n";
                }

                if (curStep == POP3_CLIENT_STEP_t::POP3_CLIENT_SEND_STAT)
                {
                        return "STAT \r\n";
                }

                if (curStep == POP3_CLIENT_STEP_t::POP3_CLIENT_LIST_ALL)
                {
                        return "LIST \r\n";
                }
                if (curStep == POP3_CLIENT_STEP_t::POP3_CLIENT_LIST_UNREAD)
                {
                        return "LIST \r\n";
                }
                if (curStep == POP3_CLIENT_STEP_t::POP3_CLIENT_STEP_PRINT_EMAIL_LIST)
                {
                        {
                                std::cout << "----------------------List All-------------------------------" << std::endl;
                                for (auto item : m_unreadIndexVec)
                                {
                                        std::cout << item.m_strIndex << "------" << item.m_emailSize << std::endl;
                                }
                                std::cout << "----------------------List All-------------------------------" << std::endl;
                        }
                        return "";
                }
                if (curStep == POP3_CLIENT_STEP_t::POP3_CLIENT_GET_ONE_UNREAD)
                {
                        std::string strCmd = "RETR " + (m_unreadIndexVec.begin()->m_strIndex) + "\r\n";
                        m_strEmailCount = m_unreadIndexVec.begin()->m_emailSize;
                        m_unreadIndexVec.erase(m_unreadIndexVec.begin());
                        return strCmd;
                }
                return "";
        }
        struct Pop3ClientElem_t
        {
                Pop3ClientElem_t(POP3_CLIENT_STEP_t curStep,
                                 Pop3Code_t protoCode,
                                 POP3_CLIENT_STEP_t nextStep)
                {
                        curStep_ = curStep;
                        protoCode_ = protoCode;
                        nextStep_ = nextStep;
                }

                POP3_CLIENT_STEP_t curStep_;
                Pop3Code_t protoCode_;
                POP3_CLIENT_STEP_t nextStep_;
        };

        const static Pop3ClientElem_t Pop3Array[] =
            {
                {POP3_CLIENT_STEP_t::POP3_CLIENT_STEP_BEGIN, Pop3Code_t::POP3_OK, POP3_CLIENT_STEP_t::POP3_CLIENT_SEND_USER_NAME},
                {POP3_CLIENT_STEP_t::POP3_CLIENT_SEND_USER_NAME, Pop3Code_t::POP3_OK, POP3_CLIENT_STEP_t::POP3_CLIENT_SEND_PASS_WORD},
                {
                    POP3_CLIENT_STEP_t::POP3_CLIENT_SEND_PASS_WORD,
                    Pop3Code_t::POP3_OK,
                    POP3_CLIENT_STEP_t::POP3_CLIENT_SEND_STAT,
                },
                {POP3_CLIENT_STEP_t::POP3_CLIENT_SEND_STAT, Pop3Code_t::POP3_OK, POP3_CLIENT_STEP_t::POP3_CLIENT_LIST_ALL},
                {
                    POP3_CLIENT_STEP_t::POP3_CLIENT_LIST_UNREAD,
                    Pop3Code_t::POP3_OK,
                    POP3_CLIENT_STEP_t::POP3_CLIENT_STEP_PRINT_EMAIL_LIST,
                },
                {
                    POP3_CLIENT_STEP_t::POP3_CLIENT_STEP_PRINT_EMAIL_LIST,
                    Pop3Code_t::POP3_OK,
                    POP3_CLIENT_STEP_t::POP3_CLIENT_STEP_END,
                },
                {
                    POP3_CLIENT_STEP_t::POP3_CLIENT_LIST_ALL,
                    Pop3Code_t::POP3_OK,
                    POP3_CLIENT_STEP_t::POP3_CLIENT_STEP_PRINT_EMAIL_LIST,
                },
                {
                        POP3_CLIENT_STEP_t::POP3_CLIENT_GET_ONE_UNREAD,
                        Pop3Code_t::POP3_ANY,
                        POP3_CLIENT_STEP_t::POP3_CLIENT_GET_ONE_UNREAD,

                },
                {POP3_CLIENT_STEP_t::POP3_CLIENT_LIST_UNREAD_FINISHED, Pop3Code_t::POP3_OK, POP3_CLIENT_STEP_t::POP3_CLIENT_STEP_PRINT_EMAIL_LIST},
                {
                    POP3_CLIENT_STEP_t::POP3_CLIENT_LIST_UNREAD,
                    Pop3Code_t::POP3_OK,
                    POP3_CLIENT_STEP_t::POP3_CLIENT_STEP_PRINT_EMAIL_LIST,
                },
        };
        const int POP3_ARRAY_SIZE = sizeof(Pop3Array) / sizeof(Pop3Array[0]);
        POP3_CLIENT_STEP_t CPop3ClientHandler::GetNextCmd(const POP3_CLIENT_STEP_t curStep, const Pop3Code_t code)
        {
                for (std::size_t i = 0; i < POP3_ARRAY_SIZE; i++)
                {
                        if (Pop3Array[i].curStep_ == curStep && (code == Pop3Array[i].protoCode_ || code == POP3_ANY))
                        {
                                return Pop3Array[i].nextStep_;
                        }
                }
                return POP3_CLIENT_STEP_t::POP3_CLIENT_STEP_NO_OP;
        }
}