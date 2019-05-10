#ifndef _EXCEPTIONS_H_CA2B1CB0_F61B_408B_A386_716B9FDD0518_
#define _EXCEPTIONS_H_CA2B1CB0_F61B_408B_A386_716B9FDD0518_

#include <stdexcept>

namespace lqs
{
    namespace util
    {
        namespace cli
        {
            class CException : public std::runtime_error
            {
            public:
                CException(const std::string& errmsg):
                    std::runtime_error(errmsg.c_str())
                {}
                
                std::string GetErrMsg() { return what(); }
            };

        }
    }
}

#endif