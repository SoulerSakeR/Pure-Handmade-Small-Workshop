#include "ThreadPool.h"

ThreadPool::ThreadPool(int numThreads)
{
    // 创建指定数量的工作线程并启动
    for (int i = 0; i < numThreads; i++)
    {
        m_threads.emplace_back(std::thread(&ThreadPool::workerThread, this));
    }
}

ThreadPool::~ThreadPool()
{
    // 先通知所有工作线程停止
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_stop = true;
    }
    m_condition.notify_all();

    // 等待所有工作线程退出并回收资源
    for (auto& thread : m_threads)
    {
        thread.join();
    }
}

void ThreadPool::workerThread()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        // 等待任务或者停止信号
        m_condition.wait(lock, [this] { return !m_tasks.empty() || m_stop; });

        // 如果收到停止信号并且任务队列为空，则退出线程
        if (m_stop && m_tasks.empty())
        {
            return;
        }

        // 取出一个任务并执行
        auto task = std::move(m_tasks.front());
        m_tasks.pop();
        lock.unlock();
        task();
    }
}
void ThreadPool::test() {
    std::cout << "meishi" << std::endl;
}