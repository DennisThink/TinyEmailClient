#ifndef _TINY_EMAIL_CLIENT_H_
#define _TINY_EMAIL_CLIENT_H_
#include <string>
void SendEmail(const std::string strUserName, std::string strPassword, std::string strReceiver, std::string Content,std::string strSubject);
#endif