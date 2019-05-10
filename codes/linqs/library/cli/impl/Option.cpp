#include "Option.h"
using _OPTION_H_7EE0E3D4_2579_459D_AFF7_BAA845AAEF5D_NS;


/** COption 默认实现 */
class OptionDefaultImpl
{


private:
    static const std::string SHORT_OPT_PREFIX;
    static const std::string LONG_OPT_PREFIX;

    std::string m_shortOpt;			///< 短选项，使用 SHORT_OPT_PREFIX 前缀标识，如 -h
    std::string m_longOpt;			///< 长选项，使用 LONG_OPT_PREFIX 前缀标识，如 --help
    std::string m_optDesc;			///< 选项描述性说明
    bool m_required;				///< 是否是必须选项  TODO: 不该和 Option 绑定。应通过 OptionGroup 描述
    bool m_hasArgument;				///< 是否有参数
    std::string m_argumentName;		///< 参数名（如果有参数）
    std::string m_argumentDesc;		///< 参数描述（如果有参数）
    bool m_hasDefaultArgument;		///< 是否有默认参数值。当有默认参数值时，命令行参数没设置该选项，则使用默认参数值
    std::string m_defaultArgument;	///< 默认参数值
};



virtual bool COption::IsRequired() const { return m_required; }
virtual bool COption::HasArgument() const { return m_hasArgument; }
virtual bool COption::HasDefaultArgument() const { return m_hasDefaultArgument; }
virtual std::string COption::GetShortOpt() const { return m_shortOpt; }
virtual std::string COption::GetLongOpt() const { return m_longOpt; }
virtual std::string COption::GetOptDesc() const { return m_optDesc; }
virtual std::string COption::GetArgumentName() const { return m_argumentName; }
virtual std::string COption::GetArgumentDesc() const { return m_argumentDesc; }
virtual std::string COption::GetDefaultArgument() const { return m_defaultArgument; }









/************/
const std::string COption::SHORT_OPT_PREFIX = "-";
const std::string COption::LONG_OPT_PREFIX = "--";

//重置：清除 opt 全部设置，Required 置为 false，argument 相关设置清除（bool 字段置为 false）
COption& COption::Reset() {
    SetOpt("", "", "");
    SetRequired(false);
    ResetArgument();

    return *this;
}
//required 为 true 时，表示必须设置选项，如果存在 argument，默认值无意义，清楚默认值状态
COption& COption::SetRequired(bool required) { 
    m_required = required; 
    if (m_required) {
        m_hasDefaultArgument = false;
        m_defaultArgument.clear();
    }

    return *this;
}
COption& COption::COption::ResetArgument() {
    m_hasArgument = false;
    m_argumentName.clear();
    m_argumentDesc.clear();
    m_hasDefaultArgument = false;
    m_defaultArgument.clear();

    return *this;
}
COption& COption::SetArgument(const std::string& name, const std::string& desc) {
    m_hasArgument = true;
    m_argumentName = name;
    m_argumentDesc = desc;

    m_hasDefaultArgument = false;
    m_defaultArgument.clear();

    return *this;
}
//如果设置了默认值，则清除 required 状态（置为 false）
COption& COption::SetDefaultArgument(const std::string& defaultArgument) {
    m_hasDefaultArgument = true;
    m_defaultArgument = defaultArgument;
    m_required = false;

    return *this;
}
COption& COption::SetOpt(const std::string& shortOpt, const std::string& longOpt, const std::string& desc) { 
    m_shortOpt = shortOpt; 
    m_longOpt = longOpt;
    m_optDesc = desc;

    return *this;
}

