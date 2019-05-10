#ifndef _COMMAND_H_7E2D4FB4_77AC_485B_99E0_28A7593E8BA6_
#define _COMMAND_H_7E2D4FB4_77AC_485B_99E0_28A7593E8BA6_
/** 命令行解析
	
    @brief 负责解析命令行参数，并保存解析结果
	@author lqs
	@date 2017-3-27
*/


#include "Options.h"
#include "Exception.h"

//package lqs.util.cli
namespace lqs
{
    namespace util
    {
        namespace cli
        {
            class CCommand
            {
            public:
                typedef CException CInvalidInputException;
                typedef CException CInvalidOptionException;
            
                typedef std::map<std::string, std::string> shortopt_value_map_t;


                CCommand() {};
                CCommand(const COptions& options) :
                    m_options(options)
                {}
                //~CCommand();
                void SetOptions(const COptions& options) { m_options = options; }

                void PrintUsage(const char* appDesc = NULL) const { PrintUsage(m_options, appDesc); }
                void PrintUsage(const COptions& options, const char* appDesc = NULL) const;
                void PrintParseResult() const;

                //必须要有的 Option 不存在算错误；Option 有参数(argument)，而命令行未提供的，算错误；未定义的 Option 忽略
                void ParseCmd(int argc, char* argv[]) throw(CInvalidInputException, CInvalidOptionException) { return ParseCmd(argc, argv, m_options); }
                void ParseCmd(int argc, char* argv[], const COptions& options) throw(CInvalidInputException, CInvalidOptionException);

                //检查参数解析结果：是否设置了某个选项
                bool IsSetOption(const std::string& shortOpt) const;

                //检查参数解析结果：是否设置了某个选项
                bool IsSetOptionByLong(const std::string& longOpt) const ;

                std::string GetOptionArgument(const std::string& shortOpt) const;

                std::string GetOptionArgumentByLong(const std::string& longOpt) const;

                //添加示例说明：这些信息会在打印帮助信息时，在命令行选项和参数的说明之后展示
                void AddExamples();
                //添加额外说明：这些信息会在打印帮助信息时，在命令行选项和参数的说明之前展示
                void AddNote();
            private:
                /**
                    从 value 字符串解析获取 opt/long-opt

                    @param value	待解析字符串
                    @param [out] isLongOpt		true 是 long opt; false 是 short opt
                    @param [out] opt			配置 isLongOpt，如果 isLongOpt 是 true，opt 表示的是 long opt 的字符串；如果 isLongOpt 是 false，opt 表示的是 short opt 的字符串，其第一个（也是唯一一个）字符即是 short opt
                                                保证方法返回 true 时，opt.size() >= 1
                    @return bool	字符串是否是一个有效的 opt/long-opt。true 是；false 不是
                */
                bool GetOpt(const char* value, bool& isLongOpt, std::string& opt) const;

                /**
                    数据格式化：
                    首行：		{some content} {space}      {one line}
                    非首行：		{left space for format}     {another line}
                                                            ...
                */
                void FormatDataWithLeftShift(
                    const std::string& fistLineContent, 
                    const int leftShiftLen, 
                    const int dataOneLineLen, 
                    const std::string& data, 
                    std::string& result) const ;

                std::string m_appName;
                COptions m_options;
                shortopt_value_map_t m_optionValues;	//key 是 shortopt，如果 value 为空，表示该选项没有参数值
            };
        }
    }
}




#endif