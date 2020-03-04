#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include "Logger.h"
#include "Singleton.h"
using namespace yazi::utility;

#include "Task.h"
#include "TaskDispatcher.h"
using namespace yazi::thread;

class EchoTask : public Task
{
public:
    EchoTask(const char * input) : Task((void *)input) {}
    virtual ~EchoTask() {}

    virtual void run()
    {
        printf("echo task run\n");
        const char * input = static_cast<const char *>(m_data);
        printf("echo: %s\n", input);
    }

    virtual void destroy()
    {
        printf("echo task destory\n");
        delete[] static_cast<char *>(m_data);
        delete this;
    }
};

int main()
{
    // init logger
    const string logfile = "./main.log";
    Logger::instance()->open(logfile);

    // init the task queue and thread pool
    int threads = 1024;
    Singleton<TaskDispatcher>::instance()->init(threads);

    while (true)
    {
        char * input = new char[1024];
        memset(input, 0, 1024);
        std::cin.getline(input, 1024);

        if (*input == 0)
        {
            continue;
        }

        Task* task = new EchoTask(input);
        Singleton<TaskDispatcher>::instance()->assign(task);
    }

    return 0;
}