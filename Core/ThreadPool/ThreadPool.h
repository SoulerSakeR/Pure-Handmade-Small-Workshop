#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool
{
public:
    // 构造函数，创建线程池并启动线程
    ThreadPool(int numThreads = std::thread::hardware_concurrency());
    // 析构函数，停止线程并回收资源
    ~ThreadPool();
    // 向线程池中添加任务
	template <typename Func, typename... Args>
	void enqueue(Func&& func, Args &&... args)
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			// 将任务加入任务队列
			m_tasks.emplace(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
		}
		// 唤醒一个线程来执行任务
		m_condition.notify_one();
	}

    void test();

private:
    // 工作线程函数，每个线程都会执行该函数
    void workerThread();
    // 线程池中的线程
    std::vector<std::thread> m_threads;
    // 任务队列
    std::queue<std::function<void()>> m_tasks;
    // 互斥量，保证线程安全
    std::mutex m_mutex;
    // 条件变量，用于线程的阻塞和唤醒
    std::condition_variable m_condition;
    // 停止标志，用于线程的退出
    bool m_stop = false;
};


