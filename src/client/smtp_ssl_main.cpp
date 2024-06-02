#include <iostream>
#include "params_struct.h"
#include "send_mail.h"
int main(int argc, char** argv)
{
    SendEmailParam_t result = ParseSendParam(argc, argv);
    if (result._bParseSucceed)
    {
        SendEmailSSL(result._strUserAddress,
            result._strUserPassword,
            result._strRecvAddr,
            result._strCopyReceiver,
            result._strContext,
            result._strSubject,
            result._bDebug,
            result._strServerAddr,
            result._strServerPort);
    }
    return 0;
}