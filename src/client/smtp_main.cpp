
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
#include <iostream>
#include "send_mail.h"
#include "params_struct.h"
//#include "clipp.h"
int main(int argc, char *argv[])
{
    SendEmailParam_t result = ParseSendParam(argc, argv);
    if(result._bParseSucceed)
    {
        SendEmail(result._strUserAddress,
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
