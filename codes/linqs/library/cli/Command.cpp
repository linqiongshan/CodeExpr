#include <cstdio>
#include "Command.h"

using namespace lqs::util::cli;
using namespace std;

//����������������Ƿ�������ĳ��ѡ��
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

//����������������Ƿ�������ĳ��ѡ��
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

		{opt prefix space length} ��opt������ shortopt��longopt����ʾ��ǰ����ӵ��Ʊ�ո����
		{opt and argument length} ��opt �� argument ��ʾ�ĳ������ơ������������Ҫ������� opt desc
		{opt desc length} ��opt ������Ϣ���г������ơ����������Ҫ�Զ����У�����һ��ǰ���貹��һ���Ŀո������Զ�������������ո���=lengthof({opt prefix space length} {1 space} {opt and argument length} {1 space})
		{argument prefix space length} ����� option �� argument������һ����� argument ������ǰ����ӵ��Ʊ�ո������Ӧ�ñ� opt ��ʾ��ƫ�����ɸ��ո�
		{argument length} ��argument name��default argument �ĳ��ȡ�����������軻������� argument desc
		{argument desc length} ��argument �����ĵ��г������ơ�������ʽ���� opt desc
	*/

	static const int OPT_PREFIX_SPACE_LEN = 4;
	static const int OPT_WITH_ARGUMENT_LEN = 40;
	static const int OPT_DESC_LEN_PER_LINE = 70;
	static const int ARG_PREFIX_SPACE_LEN = OPT_PREFIX_SPACE_LEN + 4;
	static const int ARG_LEN = OPT_WITH_ARGUMENT_LEN - (ARG_PREFIX_SPACE_LEN - OPT_PREFIX_SPACE_LEN);
	static const int ARG_DESC_LEN_PER_LINE = OPT_DESC_LEN_PER_LINE;
	//option ��������ʱ��������Ǻ� opt ����ͬһ��ʱ�������Ҫƫ�ƵĿո���
	static const int OPT_DESC_LEFT_SHIFT_SPACE_LEN = OPT_WITH_ARGUMENT_LEN + OPT_PREFIX_SPACE_LEN + 2/*opt,argument�м�һ���ո�argument,opt-desc�м�һ���ո�*/;
	//argument ��������ʱ
	static const int ARG_DESC_LEFT_SHIFT_SPACE_LEN = OPT_DESC_LEFT_SHIFT_SPACE_LEN;


	std::string cmdLineDesc = std::string("Usage: ") + m_appName;
	std::string optionsDesc;	//ȫ�� COption ��������Ϣ

	std::string optionDescOpt;	//һ�� COption ������Ϣ�� opt ���֣� short-opt, long-opt, argument-name, opt-desc
	std::string optionDescArg;	//һ�� COption ������Ϣ�� arg ���֣� argument-name��default-argument��argument-desc

	const COptions::shortopt_option_map_t& optionsDef = options.GetOptionsMap();
	for (COptions::shortopt_option_map_t::const_iterator itorOption = optionsDef.begin();
		itorOption != optionsDef.end();
		++itorOption)
	{
		const COption& option = itorOption->second;

		//command line desc����ѡ option ʹ�� [] ������argument ʹ�� <> ����
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
		optionsDesc.append("\n");	//option �м��һ��
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
				//�������ݳ����ģ�ֱ������������� data ���ݣ�ע��Ҫ�����ߵĿո�
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
			//���ı���Ի��з���ӵ�Ӱ�죺һ�����Ŀ��ܰ�������ֽڣ���ӻ��з���λ�ÿ���������һ�������ַ����м�
			//	�ᵼ�������ַ������Ƶ�ǰһ���ֺ� "\n" ��ϳ��µ��ַ������ǻ��У����������ֽ�˳����󣬿��ܵ�������
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
		throw CInvalidInputException("�����в�������(argc)С��0");
	}
	m_appName = argv[0];

	bool isLongOpt;
	std::string opt;

	//�����в������������󳡾��������в������� COptions ��δ�����ѡ��ظ�����
	for (int optionPos = 1; optionPos < argc; ) 
	{
		if (!GetOpt(argv[optionPos], isLongOpt, opt)) 
		{
			throw CInvalidOptionException(string("").append(argv[optionPos]).append("����һ��ѡ��"));
		}
		else 
		{		
			//��� opt �Ƿ��� COptions ������ѡ�����
			if (
				isLongOpt ?
					(!options.HasOptionByLong(opt)) :
					(!options.HasOption(opt))
			)
			{
				throw CInvalidOptionException(string("").append(argv[optionPos]).append("ѡ��δ����"));
			}

			//��ȡ shortOpt
			std::string shortOpt = opt;
			if (isLongOpt) 
			{
				shortOpt = options.GetOptionByLong(opt).GetShortOpt();
			}

			//�ж��Ƿ��ظ�����
			if (IsSetOption(shortOpt)) {
				throw CInvalidOptionException(string("").append(argv[optionPos]).append("�ظ�����"));
			}


			//�жϽ��ڵ���һ�� arg �ǲ��� option�����ǵĻ���������Ϊ��ǰ option �� argument��Ȼ�� optionPos+=2���ǵĻ���optionPos+=1����������
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
			//û����һ������������һ�������� Option����ֱ�ӱ��浱ǰ�� option��û�� argument��optionPos+=1
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

	//У���������Ƿ����� COptions ���塣���󳡾���Options �ж���Ϊ required ��ѡ��û�����ã�Options ����Ϊ�� argument ��ѡ��û�� argument ֵ��Options ����Ϊû�� argument ��ѡ�������� argument ֵ
	//ͬʱ����� COption ��Ĭ�� argument����������û�����ã���ʹ��Ĭ��ֵ
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
                throw CInvalidOptionException(string("").append(option.GetShortOpt()).append("�����ѡ��û������"));
			}
			else
			{
				//��� COption ��Ĭ��ֵ���������в�������û�����ã���ʹ��Ĭ��ֵ��Ϊѡ�����ֵ
				if (option.HasDefaultArgument()) 
				{
					m_optionValues.insert(
						std::make_pair(
							option.GetShortOpt(),
							option.GetDefaultArgument()
						)
					);
				}
				//�Ǳ����ѡ������в���û������ʱ����û��Ĭ��ֵ��ʲô������������
				continue;
			}
		}
		else
		{
			if (option.HasArgument() && GetOptionArgument(option.GetShortOpt()) == "") 
            {
                throw CInvalidOptionException(string("").append(option.GetShortOpt()).append("ѡ����Ҫ���ò���ֵ"));
			}

			if (!option.HasArgument() && GetOptionArgument(option.GetShortOpt()) != "") 
            {
				throw CInvalidOptionException(string("").append(option.GetShortOpt()).append("ѡ���Ҫ���ò���ֵ"));
			}
		}
	}

}
