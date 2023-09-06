#include <iostream>
#include "send_mail.h"
#include "recv_mail.h"
#include "clipp.h"
int main(int argc, char *argv[])
{
    {
        using namespace clipp;
        std::string strUser;
        std::string strPassword;
        std::string strReciver;
        std::string strContent;
        std::string strSubject;
        auto cli = (
                    required("-u", "--user").doc("the account you used for send email")&value("user",strUser),
                    required("-p", "--password").doc("the password of the account")&value("password",strPassword),
                    required("-r", "--receiver").doc("email address to receive email")&value("receiver",strReciver),
                    required("-t", "--context").doc("email content")&value("context",strContent),
                    required("-s", "--subject").doc("email subject")&value("subject",strSubject));

        if (!parse(argc, argv, cli))
        {
            std::cout << make_man_page(cli, argv[0])<<std::endl;
            return 0;
        }
        else
        {
            SendEmail(strUser, strPassword, strReciver, strContent);
        }
    }
    return 0;
}
