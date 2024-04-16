#include <iostream>
#include "imap_client_handler.h"
#include <iostream>
#include "recv_mail.h"
#include "clipp.h"
int main(int argc, char* argv[])
{
    {
        using namespace clipp;
        std::string strUser;
        std::string strPassword;
        std::string strServer;
        std::string strPort = "143";
        bool bDebugOn = false;
        bool bSetPort = false;
        auto cli = (
            required("-u", "--user").doc("the account you used for send email") & value("user", strUser),
            required("-p", "--password").doc("the password of the account") & value("password", strPassword),
            option("-d", "--debug").doc("enable debug mode").set(bDebugOn),
            option("-s", "--server").doc("imap server addr").set(strServer) & opt_value("server", strServer),
            option("-pp", "--port").doc("imap server port").set(bSetPort) & opt_value("port", strPort));
        if (!parse(argc, argv, cli))
        {
            std::cout << make_man_page(cli, argv[0]) << std::endl;
            return 0;
        }
        else
        {
            ImapRecvEmail(strUser, strPassword,strServer,strPort,bDebugOn);
        }
    }
    return 0;
}