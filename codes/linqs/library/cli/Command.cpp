#include <cstdio>
#include "Command.h"

using namespace lqs::util::cli;
using namespace std;

//检查参数解析结果：是否设置了某个选项
bool CCommand::IsSetOption(const std::string& shortOpt) const 
{
    if (m_optionValues.find(shortOpt) == m_optionValues.end()) 
    {
        return false;
    }
    else {
        return true;
    }
}

//检查参数解析结果：是否设置了某个选项
bool CCommand::IsSetOptionByLong(const std::string& longOpt) const 
{
    if (!m_options.HasOptionByLong(longOpt)) 
    {
        return false;
    }
    else {
        return IsSetOption(m_options.GetOptionByLong(longOpt).GetShortOpt());
    }
}

std::string CCommand::GetOptionArgument(const std::string& shortOpt) const 
{
    if (IsSetOption(shortOpt)) 
    {
        return m_optionValues.find(shortOpt)->second;
    }
    else {
        return "";
    }
}

std::string CCommand::GetOptionArgumentByLong(const std::string& longOpt) const 
{
    if (!m_options.HasOptionByLong(longOpt)) 
    {
        return "";
    }
    else {
        return GetOptionArgument(m_options.GetOptionByLong(longOpt).GetShortOpt());
    }
}

void CCommand::PrintUsage(const COptions & options, const char* appDesc) const
{
	if (NULL != appDesc) 
    {
		printf("\n[[[**********\n%s\n**********]]]\n", appDesc);
	}

	/** COption Description Format
		{opt prefix space length} {1 space} {opt and argument length} {1 space} {opt desc length}
		    {argument prefix space length} {argument length} {1 space} {argument desc length}

		{opt prefix space length} ：opt（包括 shortopt，longopt）提示行前面添加的制表空格个数
		{opt and argument length} ：opt 和 argument 提示的长度限制。如果超长，需要换行添加 opt desc
		{opt desc length} ：opt 描述信息单行长度限制。如果超长，要自动换行，且下一行前面需补充一定的空格数，以对其多行描述，空格数=lengthof({opt prefix space length} {1 space} {opt and argument length} {1 space})
		{argument prefix space length} ：如果 option 有 argument，另起一行添加 argument 描述，前面添加的制表空格个数，应该比 opt 提示行偏右若干个空格
		{argument length} ：argument name，default argument 的长度。如果超长，需换阿红添加 argument desc
		{argument desc length} ：argument 描述的单行长度限制。操作方式类似 opt desc
	*/

	static const int OPT_PREFIX_SPACE_LEN = 4;
	static const int OPT_WITH_ARGUMENT_LEN = 40;
	static const int OPT_DESC_LEN_PER_LINE = 70;
	static const int ARG_PREFIX_SPACE_LEN = OPT_PREFIX_SPACE_LEN + 4;
	static const int ARG_LEN = OPT_WITH_ARGUMENT_LEN - (ARG_PREFIX_SPACE_LEN - OPT_PREFIX_SPACE_LEN);
	static const int ARG_DESC_LEN_PER_LINE = OPT_DESC_LEN_PER_LINE;
	//option 多行描述时，如果不是和 opt 放在同一行时，左侧需要偏移的空格数
	static const int OPT_DESC_LEFT_SHIFT_SPACE_LEN = OPT_WITH_ARGUMENT_LEN + OPT_PREFIX_SPACE_LEN + 2/*opt,argument中间一个空格，argument,opt-desc中间一个空格*/;
	//argument 多行描述时
	static const int ARG_DESC_LEFT_SHIFT_SPACE_LEN = OPT_DESC_LEFT_SHIFT_SPACE_LEN;


	std::string cmdLineDesc = std::string("Usage: ") + m_appName;
	std::string optionsDesc;	//全部 COption 的描述信息

	std::string optionDescOpt;	//一个 COption 描述信息的 opt 部分： short-opt, long-opt, argument-name, opt-desc
	std::string optionDescArg;	//一个 COption 描述信息的 arg 部分： argument-name，default-argument，argument-desc

	const COptions::shortopt_option_map_t& optionsDef = options.GetOptionsMap();
	for (COptions::shortopt_option_map_t::const_iterator itorOption = optionsDef.begin();
		itorOption != optionsDef.end();
		++itorOption)
	{
		const COption& option = itorOption->second;

		//command line desc：可选 option 使用 [] 包裹。argument 使用 <> 包裹
		cmdLineDesc.append(" ");
		if (!option.IsRequired()) {
			cmdLineDesc.append("[");
		}

		cmdLineDesc.append(COption::GetShortOptPrefix()).append(option.GetShortOpt());
		if (option.HasArgument()) {
			cmdLineDesc.append(" <").append(option.GetArgumentName()).append(">");
		}

		if (!option.IsRequired()) {
			cmdLineDesc.append("]");
		}

		//{opt-left-space-prefix, opt(short+long), argument-name, 1 space}
		optionDescOpt
			.append(OPT_PREFIX_SPACE_LEN, ' ')
			.append(COption::GetShortOptPrefix())
			.append(option.GetShortOpt())
			.append(",")
			.append(COption::GetLongOptPrefix())
			.append(option.GetLongOpt());
		
		if (option.HasArgument()) 
		{
			optionDescOpt.append(" <").append(option.GetArgumentName()).append(">");
		}
		optionDescOpt.append(" ");
		
		//{opt desc}
		FormatDataWithLeftShift(
			std::string(optionDescOpt), 
			OPT_DESC_LEFT_SHIFT_SPACE_LEN, 
			OPT_DESC_LEN_PER_LINE, 
			option.GetOptDesc(), 
			optionDescOpt);
		
		if (option.HasArgument()) 
		{
			//{argument-left-space-prefix, argument-name, default-argument, 1 space}
			optionDescArg
				.append(ARG_PREFIX_SPACE_LEN, ' ')
				.append("<")
				.append(option.GetArgumentName())
				.append(">");
			if (option.HasDefaultArgument())
			{
				optionDescArg.append(" [default: ").append(option.GetDefaultArgument()).append("]");
			}
			optionDescArg.append(" ");

			//{argument desc}
			FormatDataWithLeftShift(
				std::string(optionDescArg),
				ARG_DESC_LEFT_SHIFT_SPACE_LEN,
				ARG_DESC_LEN_PER_LINE,
				option.GetArgumentDesc(),
				optionDescArg);
		}

		optionsDesc.append(optionDescOpt).append("\n");
		if (!optionDescArg.empty())
		{
			optionsDesc.append(optionDescArg).append("\n");
		}
		optionsDesc.append("\n");	//option 中间空一行
		optionDescOpt.clear();
		optionDescArg.clear();
	}

	printf("\n%s\n\n%s", cmdLineDesc.c_str(), optionsDesc.c_str());
}

void CCommand::FormatDataWithLeftShift(
	const std::string& fistLineContent, 
	const int leftShiftLen, 
	const int dataOneLineLen, 
	const std::string& data, 
	std::string& result) const
{
	result.clear();
	result.append(fistLineContent);

	int hadAppendDataLen = 0;
	bool isFirstLine = true;
	while (hadAppendDataLen < data.size())
	{
		if (isFirstLine)
		{
			isFirstLine = false;
			if (result.size() < leftShiftLen)
			{
				result.append(leftShiftLen - result.size(), ' ');
			}
			else
			{
				//首行内容超长的，直接起新行来添加 data 数据（注意要添加左边的空格）
				result.append("\n");
				result.append(leftShiftLen, ' ');
			}
		}
		else
		{
			result.append(leftShiftLen, ' ');
		}

		if (data.size() - hadAppendDataLen > dataOneLineLen) 
		{
			//中文编码对换行符添加的影响：一个中文可能包含多个字节，添加换行符的位置可能正好在一个中文字符的中间
			//	会导致中文字符二进制的前一部分和 "\n" 组合成新的字符，而非换行，后面内容字节顺序错误，可能导致乱码
			result.append(data.substr(hadAppendDataLen, dataOneLineLen)).append("    \n");
			hadAppendDataLen += dataOneLineLen;
		}
		else
		{
			result.append(data.substr(hadAppendDataLen));
			break;
		}
	}
}

void CCommand::PrintParseResult() const
{
	printf("\nCommand Parse Result: \n");
	for (shortopt_value_map_t::const_iterator itor = m_optionValues.begin();
		itor != m_optionValues.end();
		++itor)
	{
		printf("COption: %-15s ", itor->first.c_str());
		if (itor->second.size() > 0) 
		{
			printf("Argument: %s", itor->second.c_str());
		}
		printf("\n");
	}
	printf("\n");
}

bool CCommand::GetOpt(const char* value, bool& isLongOpt, std::string& opt) const
{
	if (NULL == value) 
    {
		return false;
	}
	opt.clear();
	std::string valueString = value;

	if (valueString.size() > COption::GetLongOptPrefix().size()
		&& valueString.substr(0, COption::GetLongOptPrefix().size()) == COption::GetLongOptPrefix())
	{
		isLongOpt = true;
		opt = valueString.substr(COption::GetLongOptPrefix().size());
	}
	else if (valueString.size() > COption::GetShortOptPrefix().size()
		&& valueString.substr(0, COption::GetShortOptPrefix().size()) == COption::GetShortOptPrefix())
	{
		isLongOpt = false;
		opt = valueString.substr(COption::GetShortOptPrefix().size());
	}
	else {
		return false;
	}

	return true;
}

void CCommand::ParseCmd(int argc, char* argv[], const COptions& options) throw(CInvalidInputException, CInvalidOptionException)
{
	m_optionValues.clear();

	if (argc <= 0) 
    {
		throw CInvalidInputException("命令行参数个数(argc)小于0");
	}
	m_appName = argv[0];

	bool isLongOpt;
	std::string opt;

	//命令行参数解析。错误场景：命令行参数包括 COptions 中未定义的选项；重复设置
	for (int optionPos = 1; optionPos < argc; ) 
	{
		if (!GetOpt(argv[optionPos], isLongOpt, opt)) 
		{
			throw CInvalidOptionException(string("").append(argv[optionPos]).append("不是一个选项"));
		}
		else 
		{		
			//检查 opt 是否在 COptions 包含的选项定义中
			if (
				isLongOpt ?
					(!options.HasOptionByLong(opt)) :
					(!options.HasOption(opt))
			)
			{
				throw CInvalidOptionException(string("").append(argv[optionPos]).append("选项未定义"));
			}

			//获取 shortOpt
			std::string shortOpt = opt;
			if (isLongOpt) 
			{
				shortOpt = options.GetOptionByLong(opt).GetShortOpt();
			}

			//判断是否重复设置
			if (IsSetOption(shortOpt)) {
				throw CInvalidOptionException(string("").append(argv[optionPos]).append("重复设置"));
			}


			//判断紧邻的下一个 arg 是不是 option，不是的话，将其作为当前 option 的 argument，然后 optionPos+=2；是的话，optionPos+=1，继续处理
			if ((optionPos + 1 < argc) && !GetOpt(argv[optionPos + 1], isLongOpt, opt))
			{
				m_optionValues.insert(
					std::make_pair(
						shortOpt,
						argv[optionPos + 1]
					)
				);

				optionPos += 2;
			}
			//没有下一个参数，或下一个参数是 Option，则直接保存当前的 option，没有 argument。optionPos+=1
			else
			{
				m_optionValues.insert(
					std::make_pair(
						shortOpt,
						""
					)
				);

				optionPos += 1;
			}
		}
	}

	//校验解析结果是否满足 COptions 定义。错误场景：Options 中定义为 required 的选项没有设置；Options 定义为有 argument 的选项没有 argument 值；Options 定义为没有 argument 的选项设置了 argument 值
	//同时，如果 COption 有默认 argument，而命令行没有设置，则使用默认值
	const COptions::shortopt_option_map_t& optionsDef = options.GetOptionsMap();
	for (COptions::shortopt_option_map_t::const_iterator itor = optionsDef.begin();
		itor != optionsDef.end();
		++itor)
	{
		const COption& option = itor->second;

		if (!IsSetOption(option.GetShortOpt()))
		{
			if (option.IsRequired()) 
			{
                throw CInvalidOptionException(string("").append(option.GetShortOpt()).append("必须的选项没有设置"));
			}
			else
			{
				//如果 COption 有默认值，而命令行参数中又没有设置，则使用默认值作为选项参数值
				if (option.HasDefaultArgument()) 
				{
					m_optionValues.insert(
						std::make_pair(
							option.GetShortOpt(),
							option.GetDefaultArgument()
						)
					);
				}
				//非必须的选项，命令行参数没有设置时，又没有默认值，什么都不处理，跳过
				continue;
			}
		}
		else
		{
			if (option.HasArgument() && GetOptionArgument(option.GetShortOpt()) == "") 
            {
                throw CInvalidOptionException(string("").append(option.GetShortOpt()).append("选项需要设置参数值"));
			}

			if (!option.HasArgument() && GetOptionArgument(option.GetShortOpt()) != "") 
            {
				throw CInvalidOptionException(string("").append(option.GetShortOpt()).append("选项不需要设置参数值"));
			}
		}
	}

}
