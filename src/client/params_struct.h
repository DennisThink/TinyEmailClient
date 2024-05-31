#ifndef _PARAMS_STRUCT_H_
#define _PARAMS_STRUCT_H_
#include <string>
struct RecvEmailParams_t
{
	std::string _strUserAddress;
	std::string _strUserPassword;
	std::string _strServerAddr;
	std::string _strServerPort;
	bool		_bDebug;
	bool        _bParseSucceed;
	RecvEmailParams_t()
	{
		_strUserAddress.clear();
		_strUserPassword.clear();
		_strServerAddr.clear();
		_strServerPort.clear();
		_bDebug = false;
		_bParseSucceed = false;
	}
};


struct SendEmailParam_t
{
	std::string _strUserAddress;
	std::string _strUserPassword;
	std::string _strServerAddr;
	std::string _strServerPort;
	std::string _strRecvAddr;
	std::string _strSubject;
	std::string _strCopyReceiver;
	std::string _strContext;

	bool		_bDebug;
	bool        _bParseSucceed;
	SendEmailParam_t()
	{
		_strUserAddress.clear();
		_strUserPassword.clear();
		_strServerAddr.clear();
		_strServerPort.clear();
		_bDebug = false;
		_bParseSucceed = false;
	}


};
SendEmailParam_t ParseSendParam(int argc, char* argv[]);
RecvEmailParams_t ParseRecvParam(int argc, char* argv[]);
#endif