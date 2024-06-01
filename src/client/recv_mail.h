#ifndef _TINY_EMAIL_CLIENT_RECV_H_
#define _TINY_EMAIL_CLIENT_RECV_H_
#include <string>
void Pop3RecvEmail(const std::string strUserName, std::string strPassword,  std::string strServer, std::string strPort = "110",bool bDebug=false);
void Pop3RecvEmailSSL(const std::string strUserName, std::string strPassword,std::string strServer,std::string strPort = "110",bool bDebug = false);
void ImapRecvEmail(const std::string strUserName, std::string strPassword, std::string strServer, std::string strPort, bool bDebug = false);
void ImapRecvEmailSSL(const std::string strUserName, std::string strPassword, std::string strServer, std::string strPort, bool bDebug = false);
#endif