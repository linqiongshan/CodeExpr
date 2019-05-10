/*
 * @date: 2018-10-06 07:53:29
 * @author : lin.qs
 * @email : github.com/maxthon147532
 * -----
 * @brief Option定义一个命令行选项的格式，主要包括选项前缀，选项名称和选项参数
 *      The Option file define the format of a command line option. Include prefix, option name and option arguments
 */

#ifndef _OPTION_H_7EE0E3D4_2579_459D_AFF7_BAA845AAEF5D_
#define _OPTION_H_7EE0E3D4_2579_459D_AFF7_BAA845AAEF5D_

#include "Namespace.h"
_9C91EFED_B134_14C2_1581_A233801CDD57_NS_BEGIN

/** 定义一个命令行选项
 * 
 *  一个命令行选项由以下几个元数据组成
 *      > required
 *      > argument
 *      > default argument
 * 
 *  设置 default argument 和设置 required 为 true 是互相冲突的
 *      提供了默认参数值，选项就不是必须设置的，不设置时会使用默认值；设置了 required 为 true，就要求必须设置选项，提供默认值是没有意义的
 *      实现上，设置其中一个，会清除另一个的设置，因此如果同时设置 default argument 和 required，其行为未定义（依赖设置顺序）
*/
class COption
{
public:
    COption() :
        m_required(false),
        m_hasArgument(false),
        m_hasDefaultArgument(false)
    {}

    COption(const std::string& shortOpt, const std::string& longOpt, const std::string& desc) :
        m_shortOpt(shortOpt),
        m_longOpt(longOpt),
        m_optDesc(desc),
        m_required(false),
        m_hasArgument(false),
        m_hasDefaultArgument(false)
    {}

    virtual COption() {}

    //重置：清除 opt 全部设置，Required 置为 false，argument 相关设置清除（bool 字段置为 false）
    virtual COption& Reset();
    
    //required 为 true 时，表示必须设置选项，如果存在 argument，默认值无意义，清楚默认值状态
    virtual COption& SetRequired(bool required);
    
    //只重置 argment 相关设置
    virtual COption& ResetArgument();
    
    virtual COption& SetArgument(const std::string& name, const std::string& desc);
    
    //如果设置了默认值，则清除 required 状态（置为 false）
    virtual COption& SetDefaultArgument(const std::string& defaultArgument);
    
    /** 设置选项名和选项描述。选项名包括短选项和长选项两种 */
    virtual COption& SetOpt(const std::string& shortOpt, const std::string& longOpt, const std::string& desc);

    //命令行选项名称支持两种形式，一种短格式，一种长格式。两种格式的前缀也不一样
    static std::string GetShortOptPrefix() { return SHORT_OPT_PREFIX; }
    static std::string GetLongOptPrefix() { return LONG_OPT_PREFIX; }
    
    bool IsRequired();
    bool HasArgument();
    bool HasDefaultArgument();
    std::string GetShortOpt();
    std::string GetLongOpt();
    std::string GetOptDesc();
    std::string GetArgumentName();
    std::string GetArgumentDesc();
    std::string GetDefaultArgument();
};        

_9C91EFED_B134_14C2_1581_A233801CDD57_NS_END

#endif