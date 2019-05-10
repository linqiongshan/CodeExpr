#include "Options.h"

using namespace lqs::util::cli;

/***** COptions *****/
COptions::COptions() {}
COptions::COptions(const COptions& options) :
	m_options(options.m_options)
{}
COptions::~COptions() { m_options.clear(); }

COptions& COptions::AddOption(const COption& option, bool replaceExistOption) throw (COptionDuplicateException) 
{
	if (!replaceExistOption &&
			(m_longOpt2ShortOpt.find(option.GetLongOpt()) != m_longOpt2ShortOpt.end()
				|| m_options.find(option.GetShortOpt()) != m_options.end()
			)
		)
	{
		throw COptionDuplicateException(option.GetShortOpt(), option.GetLongOpt());
	}

	m_options[option.GetShortOpt()] = option;
	m_longOpt2ShortOpt[option.GetLongOpt()] = option.GetShortOpt();

	return *this;
}

bool COptions::HasOption(const std::string& shortOpt) const 
{
	if (m_options.find(shortOpt) != m_options.end()) 
    {
		return true;
	}
	else {
		return false;
	}
}

bool COptions::HasOptionByLong(const std::string& longOpt) const 
{
	if (m_longOpt2ShortOpt.find(longOpt) == m_longOpt2ShortOpt.end()) 
    {
		return false;
	}

	return HasOption(m_longOpt2ShortOpt.find(longOpt)->second);
}

COption COptions::GetOption(const std::string& shortOpt) const throw (COptionNotExistException) 
{
	if (HasOption(shortOpt)) 
    {
		return m_options.find(shortOpt)->second;
	}
	else {
		throw COptionNotExistException(shortOpt, "");
	}
}

COption COptions::GetOptionByLong(const std::string& longOpt) const throw (COptionNotExistException) 
{
	if (m_longOpt2ShortOpt.find(longOpt) != m_longOpt2ShortOpt.end()) 
    {
		return GetOption(m_longOpt2ShortOpt.find(longOpt)->second);
	}
	else {
		throw COptionNotExistException("", longOpt);
	}
}

const COptions::shortopt_option_map_t& COptions::GetOptionsMap() const 
{
	return m_options;
}

void COptions::Clear()
{
    m_longOpt2ShortOpt.clear();
    m_options.clear();
}