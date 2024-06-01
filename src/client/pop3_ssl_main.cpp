
/**
 * @file main.cpp
 * @author DennisThink (dennisthink@hotmail.com)
 * @brief this is a simple program to send email
 * @version 0.1
 * @date 2023-09-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "recv_mail.h"
#include "params_struct.h"
int main(int argc, char *argv[])
{
    RecvEmailParams_t result = ParseRecvParam(argc, argv);
    if(result._bParseSucceed)
    {
        Pop3RecvEmailSSL(result._strUserAddress, 
            result._strUserPassword,
            result._strServerAddr,
            result._strServerPort,
            result._bDebug);
    }

    return 0;
}
