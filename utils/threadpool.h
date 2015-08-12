#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>


class IRunner
{
public:
    virtual void run() = 0;
};

class ThreadPool
{
    public:
        ThreadPool(size_t threadsCount);
        ~ThreadPool();

        void schedule(std::shared_ptr<IRunner> runner);
        void join();
    protected:

    private:
        void threadLoop();
        std::shared_ptr<IRunner> pop();

        ThreadPool(const ThreadPool& that)=delete;
        ThreadPool& operator=(const ThreadPool& that)=delete;

    volatile bool shutDown_;
    std::vector<std::thread> threads_;
    std::vector<std::shared_ptr<IRunner>> queue_;
    std::mutex m_;
    std::condition_variable cv_;
};

#endif // THREADPOOL_H
