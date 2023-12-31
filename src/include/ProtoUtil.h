#ifndef _TINY_EMAIL_PROTO_UTIL_H_
#define _TINY_EMAIL_PROTO_UTIL_H_
#include <string>
#include <vector>
namespace tiny_email
{
    class CProtoUtil final
    {
    public:
        static std::vector<std::string> SplitStringByLine(const std::string& strValue);
        static bool SplitLine(const std::string& strInput,int& code,std::string& value,bool& bFinish);
        static std::string Base64Encode(const std::string strInput);
        static std::string Base64Decode(const std::string strInput);
        static bool ParseFromToString(const std::string strInput,std::string& name,std::string& emailAddr);
        static std::string& Trim(std::string &s);
        static std::string AddrToIp(const std::string strAddr); 
    };
}
#endif