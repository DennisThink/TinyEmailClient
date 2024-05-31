#include "recv_mail.h"
#include "params_struct.h"
int main(int argc, char* argv[])
{
    RecvEmailParams_t result = ParseRecvParam(argc, argv);
    if (result._bParseSucceed)
    {
        ImapRecvEmail(result._strUserAddress,
            result._strUserPassword,
            result._strServerAddr,
            result._strServerPort,
            result._bDebug);
    }
    return 0;
}