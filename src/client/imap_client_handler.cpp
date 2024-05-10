#include "imap_client_handler.h"
#include <iostream>
namespace tiny_email
{
	CImapClientHandler::CImapClientHandler(const std::string strUserName, const std::string strPassword):EmailClientProtoInterface(strUserName,strPassword)
	{
		m_strUserName = strUserName;
		m_strPassword = strPassword;
		m_strUserName = strUserName;
		m_strPassword = strPassword;
		std::size_t index = m_strUserName.find_first_of("@");
		m_strServerAddr = "imap." + m_strUserName.substr(index + 1);
		m_strUserName = m_strUserName.substr(0, index);
		m_nCommandIndex = 1;
		m_step = IMAP_CLIENT_BEGIN;
	}

	struct ImapClientElem_t {
		IMAP_CLIENT_STEP_T curState;
		PARSE_IMAP_RESULT curCode;
		IMAP_CLIENT_STEP_T nextState;
	};
	static ImapClientElem_t imapArray[] = {
		{IMAP_CLIENT_BEGIN,PARSE_IMAP_RESULT::PARSE_IMAP_SUCCEED,IMAP_CLIENT_GET_CAPABILITY},
		{IMAP_CLIENT_GET_CAPABILITY,PARSE_IMAP_RESULT::PARSE_IMAP_SUCCEED,IMAP_CLIENT_LOGIN},
		{IMAP_CLIENT_LOGIN,PARSE_IMAP_RESULT::PARSE_IMAP_SUCCEED,IMAP_CLIENT_SELECT_INBOX},
		{IMAP_CLIENT_SELECT_INBOX,PARSE_IMAP_RESULT::PARSE_IMAP_SUCCEED,IMAP_CLIENT_CHECK_INBOX},
		{IMAP_CLIENT_CHECK_INBOX,PARSE_IMAP_RESULT::PARSE_IMAP_SUCCEED,IMAP_CLIENT_SEARCH_ALL},
		{IMAP_CLIENT_SEARCH_ALL,PARSE_IMAP_RESULT::PARSE_IMAP_SUCCEED,IMAP_CLIENT_FETCH_ONE},
		//{IMAP_CLIENT_FETCH_ONE,PARSE_IMAP_RESULT::PARSE_IMAP_SUCCEED,IMAP_CLIENT_FETCH_ONE},
		{IMAP_CLIENT_FETCH_ONE,PARSE_IMAP_RESULT::PARSE_IMAP_FAILED,IMAP_CLIENT_LOGOUT},
		{IMAP_CLIENT_LOGIN,PARSE_IMAP_RESULT::PARSE_IMAP_SUCCEED,IMAP_CLIENT_END},
	};
	const std::size_t ARRAY_SIZE = sizeof(imapArray) / sizeof(imapArray[0]);

   std::string CImapClientHandler::GetServerAddr()
	{
	   return m_strServerAddr;
	}
    int CImapClientHandler::GetServerPort()
	{
		return 143;
	}
	int CImapClientHandler::GetServerSSLport()
	{
		return 993;
	 }

	void CImapClientHandler::OnServerCommand(const std::string strValue)
	{
		for (std::size_t i = 0; i < ARRAY_SIZE; i++)
		{
			if (imapArray[i].curState == m_step)
			{
				m_step = imapArray[i].nextState;
				break;
			}
		}
		if (m_step == IMAP_CLIENT_GET_CAPABILITY)
		{
			m_strResponse = "A01 CAPABILITY\r\n";
		}
		if (m_step == IMAP_CLIENT_LOGIN)
		{
			m_strResponse = "A02 LOGIN " + m_strUserName +" "+ m_strPassword + "\r\n";
		}
		if (m_step == IMAP_CLIENT_SELECT_INBOX)
		{
			m_strResponse = "A03 SELECT INBOX\r\n";
		}
		if (m_step == IMAP_CLIENT_CHECK_INBOX)
		{
			m_strResponse = "A04 EXAMINE INBOX\r\n";
		}
		if (m_step == IMAP_CLIENT_SEARCH_ALL)
		{
			m_strResponse = "A05 SEARCH ALL\r\n";
		}
		if (m_step == IMAP_CLIENT_FETCH_ONE)
		{
			m_strResponse = "A06 FETCH 1 RFC822\r\n";
		}
		if (m_step == IMAP_CLIENT_LOGOUT)
		{
			m_strResponse = "A07 LOGOUT\r\n";
		}
		
		return;
	}
	bool CImapClientHandler::IsServerRspCompleted(const std::string strRsp)
	{
		if (strRsp.find("\r\n") != std::string::npos)
		{
			return true;
		}
		return false;
	}
}