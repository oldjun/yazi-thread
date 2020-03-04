# yazi-thread
a c++ thread pool and task queue. Works with pthreads only.

|Author|junping|
|---|---|
|E-mail|oldjun@sina.com|
|Wechat|chenjunping1024|

## The thread pool
More specifically, there are some threads dedicated to the pool and a container of tasks. The tasks come to the pool dynamically. A task is fetched and deleted from the container when there is an idle thread. The task is then run on that thread.

A thread pool is helpful when you want to minimize time of loading and destroying threads and when you want to limit the number of parallel tasks that run simultanuasly. For example, time consuming event handlers may be processed in a thread pool to make UI more responsive.

## Implement your task
All user task should inherit from basic Task class, implement the ___run___ and ___destroy___ method. According to your need, you need just only to define your task class and implement it. For example, I defined a EchoTask class following, it simplely echo what I inputed:
```c++
#include "Task.h"
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
        m_status = DONE;
    }

    virtual void destroy()
    {
        printf("echo task destory\n");
        delete[] static_cast<char *>(m_data);
        delete this;
    }
};
```
## Echo demo

In this demo, we input from console with std::cin, then create a echo task and put it into the task queue, that's all. After that the task dispatcher will take care every thing, like assign it to the thread pool, the thread pool will assign a idle thread to run the task.
```c++
#include "Task.h"
#include "TaskDispatcher.h"

using namespace yazi::utility;
using namespace yazi::thread;

int main()
{
    // init the task queue and thread pool
    int threads = 1024;
    Singleton<TaskDispatcher>::instance()->init(threads);

    while (true)
    {
        char * input = new char[1024];
        memset(input, 0, 1024);
        std::cin.getline(input, 1024);

        // new a echo task and assign it to the task queue
        Task* task = new EchoTask(input);
        Singleton<TaskDispatcher>::instance()->assign(task);
    }

    return 0;
}
```
## Compile & Run the project
download the source code, cd into the yazi-thread project working directory, run command make && ./main 
```bash
cd ./yazi-thread
make
./main
```
