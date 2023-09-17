#ifndef _TINY_EMAIL_CLIENT_RECV_H_
#define _TINY_EMAIL_CLIENT_RECV_H_
#include <string>
void RecvEmail(const std::string strUserName, std::string strPassword,bool bDebug=false,std::string strPort="110");
#endif