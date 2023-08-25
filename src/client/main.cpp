#include <iostream>
#include "send_mail.h"
#include "recv_mail.h"
int main(int argc, char *argv[])
{
    if(argc < 4)
    {
        std::cout<<"Usage: sendemail {SenderAddr} {SenderPassword} {ReceiverAddr} {EmailContent}"<<std::endl;
        return -1;
    }
    else
    {
        std::string strUser =  argv[1];
        std::string strPassword = argv[2];
        std::string strReciver = argv[3];
        std::string strContent = argv[4];
        //SendEmail(strUser, strPassword, strReciver, strContent);
        RecvEmail(strUser, strPassword);
        std::cout << "Client " << std::endl;
    }
    return 0;
}