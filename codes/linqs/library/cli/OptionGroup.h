#ifndef _OPTIONS_H_D3CA4852_1B41_4DB1_B528_B3689757CF5C_
#define _OPTIONS_H_D3CA4852_1B41_4DB1_B528_B3689757CF5C_

#include <string>
#include <map>
#include <stdexcept>
#include "Option.h"
#include "Exception.h"

/** 一组 COption 的集合，
    
    @date 2017-3-27
    @author lqs
*/

namespace lqs
{
    namespace util
    {
        namespace cli
        {
            //一组 COption 的集合。提供按 shortOpt, longOpt 检查和获取 COption 的方法
            class COptions
            {
            public:
                //异常：指定的 option 不存在
                //	从 COptions 查找 option 时，应首先使用 hasOption 方法判断 COption 是否存在，再使用 getOption 方法获取；否则可能抛出该异常
                class COptionNotExistException : public CException
                {
                public:
                    COptionNotExistException(const std::string& shortOpt, const std::string& longOpt):
                        CException(
                            std::string("")
                                .append("COption[shortOpt=")
                                .append(shortOpt)
                                .append(", longOpt=")
                                .append(longOpt)
                                .append("] not exist")
                                .c_str()
                        )
                    {}
                };

                //异常：指定的 option 已存在，shortopt 或 longopt 相同，都有可能导致该异常
                class COptionDuplicateException : public CException
                {
                public:
                    COptionDuplicateException(const std::string& shortOpt, const std::string& longOpt) :
                        CException(
                            std::string("")
                            .append("COption[shortOpt=")
                            .append(shortOpt)
                            .append(", longOpt=")
                            .append(longOpt)
                            .append("] definition already exist")
                            .c_str()
                        )
                    {}
                };
            
                typedef std::map<std::string, std::string> longopt_shortopt_map_t;
                typedef std::map<std::string, COption> shortopt_option_map_t;

                COptions();
                COptions(const COptions& options);
                ~COptions();

                //添加一个 option 的定义
                //	replaceExistOption: 如果已存在同名 option，是否允许覆盖。[默认] false，不允许，此时如果有同名 option 方法会抛出异常; true 允许
                COptions& AddOption(const COption& option, bool replaceExistOption=false) throw (COptionDuplicateException);

                //根据 shortopt 判断是否存在一个 option 的定义
                bool HasOption(const std::string& shortOpt) const;

                //根据 longopt 判断是否存在一个 option 的定义
                bool HasOptionByLong(const std::string& longOpt) const;

                //通过 shortopt 获取一个 option 的定义。应始终先使用 hasOption 方法判断是否存在，否则可能抛出异常
                COption GetOption(const std::string& shortOpt) const throw (COptionNotExistException);

                //通过 longopt 获取一个 option 的定义。应始终先使用 hasOptionByLong 方法判断是否存在，否则可能抛出异常
                COption GetOptionByLong(const std::string& longOpt) const throw (COptionNotExistException);

                //获取内部的全部 option 定义
                const shortopt_option_map_t& GetOptionsMap() const;
                
                //清空内部存储的所有数据
                void Clear();

            private:
                //longOpt 到 shortOpt 的映射
                longopt_shortopt_map_t m_longOpt2ShortOpt;
                //shortOpt 到 COption 的映射
                shortopt_option_map_t m_options;
            };
        }
    }
}

#endif
