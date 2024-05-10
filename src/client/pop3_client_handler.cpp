#include "pop3_client_handler.h"
#include <iostream>
#include "ProtoUtil.h"
#include "CPop3ProtoCmd.h"
namespace tiny_email
{
        CPop3ClientHandler::CPop3ClientHandler(const std::string strEmailAddr, const std::string strPassword):EmailClientProtoInterface(strEmailAddr,strPassword)
        {
            std::string strEmail = strEmailAddr;
            std::size_t index = strEmail.find_first_of("@");
            m_strServerAddr = "pop3." + strEmail.substr(index + 1);
            m_strUserName = strEmail.substr(0, index);
            m_step = POP3_CLIENT_STEP_t::POP3_CLIENT_STEP_BEGIN;
        }

        bool CPop3ClientHandler::IsFinished()
        {
                return false;
        }
        std::string CPop3ClientHandler::GetServerAddr()
        {
            return m_strServerAddr;
        }

        int CPop3ClientHandler::GetServerPort()
        {
            return 110;
        }

        int CPop3ClientHandler::GetServerSSLport()
        {
            return 995;
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
                        if (strRsp.find(".\r\n") !=std::string::npos)
                        {
                            std::cout<<"-----------------------------------------------------"<<std::endl;
                            std::cout<<"MailIndex:  "<< m_nCurEmailCount  <<"   Server: "<<strRsp.length()<<"   "<<std::endl;
                            std::cout<<"-----------------------------------------------------"<<std::endl;
                            return true;
                        }
                        else
                        {
                            //std::cout<<"Index:"<<m_strCurMailIndex  <<"Size: "<<m_nCurEmailCount<<"  "<<strRsp.length()<<std::endl;
                            return false;
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
                std::size_t count = 0;//
                m_unreadIndexVec.push_back({ strIndex, count });
            }
            {
                std::cout << "----------------------List All-------------------------------" << std::endl;
                for (auto item : m_unreadIndexVec)
                {
                    std::cout << item.m_strIndex << "------" << item.m_emailSize << std::endl;
                }
                std::cout << "----------------------List All-------------------------------" << std::endl;
            }
            UpdateCurEmail();
            return;
        }
        
        

        void CPop3ClientHandler::UpdateCurEmail()
        {
            if (!m_unreadIndexVec.empty())
            {
                m_strCurMailIndex = m_unreadIndexVec.begin()->m_strIndex;
                m_nCurEmailCount = m_unreadIndexVec.begin()->m_emailSize;
                m_unreadIndexVec.erase(m_unreadIndexVec.begin());
            }
            else
            {
                m_step = POP3_CLIENT_STEP_t::POP3_CLIENT_STEP_QUIT;
            }
        }

        void CPop3ClientHandler::HandleGetOneUnread(const std::string strValue)
        {
                m_strEmailFormat += strValue;
                std::cout << "----------------------------------------" << std::endl;
                //std::cout << "EMail: " << m_strEmailFormat << std::endl;
                std::cout << "----------------------------------------" << std::endl;
                std::cout << "EMailLen: " << m_strEmailFormat.length() << "     " << m_nCurEmailCount << std::endl;
                m_strEmailFormat.clear();
                UpdateCurEmail();
             
        }

        void CPop3ClientHandler::HandleServerRsp(const std::string strValue)
        {
            if (m_step == POP3_CLIENT_STEP_t::POP3_CLIENT_LIST_ALL)
            {
                HandleListAllRsp(strValue);
                m_step = POP3_CLIENT_GET_ONE_UNREAD;
                m_step = GetNextCmd(m_step, POP3_ANY);
                m_strResponse = CmdToRequestContext(m_step);
                return;
            }
            if (m_step == POP3_CLIENT_STEP_t::POP3_CLIENT_GET_ONE_UNREAD)
            {
                HandleGetOneUnread(strValue);
                m_step = GetNextCmd(m_step, POP3_ANY);
                m_strResponse = CmdToRequestContext(m_step);
                return;
            }
            CPop3ProtoCmd curCmd = CPop3ProtoCmd::FromString(strValue);
            m_step = GetNextCmd(m_step, curCmd.GetCode());
            m_strResponse = CmdToRequestContext(m_step);

        }

        void CPop3ClientHandler::OnServerCommand(const std::string strValue)
        {
            m_strRecv += strValue;
            if (IsServerRspCompleted(m_strRecv))
            {
                std::string strRsp = m_strRecv;
                HandleServerRsp(strRsp);
                m_strRecv.clear();
            }
        }

        std::string CPop3ClientHandler::CmdToRequestContext(const POP3_CLIENT_STEP_t curStep)
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
                      std::string strCmd = "RETR " + m_strCurMailIndex + "\r\n";
                      return strCmd;
                }
                if (curStep == POP3_CLIENT_STEP_t::POP3_CLIENT_STEP_QUIT)
                {
                        return "quit\r\n";
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
                 {
                    POP3_CLIENT_STEP_t::POP3_CLIENT_STEP_QUIT,
                    Pop3Code_t::POP3_ANY,
                    POP3_CLIENT_STEP_t::POP3_CLIENT_STEP_QUIT,
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