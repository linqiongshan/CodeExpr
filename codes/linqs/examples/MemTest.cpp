#include <vector>
#include <cstring>
#include <cstdio>
#include <string>

#define LOG(fmt,...) printf(fmt, __VA_ARGS__);

int HumanReadable(unsigned long nBytes, double& nHumanReadable, std::string& unitHumanReadable)
{
    static char* s_units[] = 
    {
        "B",
        "KB",
        "MB",
        "GB",
        "TB",
        "PB"
    };
    
    nHumanReadable = nBytes;

    unsigned int unitIndex = 0;
    while (nHumanReadable >= 500.0 && unitIndex < sizeof(s_units)/sizeof(char*))
    {
        if ()
        {
            break;
        }
        else 
        {
            nHumanReadable /= 1024;
        }
    }
}

/** 堆内存申请测试 */
int HeapMem(unsigned long mallocUnit, unsigned long mallocTimes)
{
    std::vector<char*> heaps;
    for (unsigned long i = 0; i < mallocTimes; ++i)
    {
        heaps.push_back(new char[mallocUnit]);
        LOG("The [] times malloc, unit is [%ul] bytes, total are [] ");
    }
}

/** 栈内存申请测试 */
int StackMem();

/** 堆内存泄漏测试 */
int HeapMemLeak();

/** 堆内存越界 */
int HeapMemOutOfRange();

int main(int argc, char* argv[]) 
{
    char* heapData = new char[512*1024*1024];
    return 0;
}