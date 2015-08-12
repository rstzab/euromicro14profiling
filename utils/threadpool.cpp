#include "threadpool.h"

#include <iostream>

using namespace std;

ThreadPool::ThreadPool(size_t threadsCount): shutDown_(false)
{
    for(int i = 0; i <threadsCount; ++i)
    {
        threads_.emplace_back(&ThreadPool::threadLoop, this);
    }
}

ThreadPool::~ThreadPool()
{
    //cout<<"Destructing..."<<endl;
    shutDown_ = true;
    cv_.notify_all();

    // we do this to ensure that all threads receive message and not timeout.
    this_thread::sleep_for(chrono::milliseconds(10));
    cv_.notify_all();

    //cout<<"Destructing..."<<endl;

    for(auto& thread : threads_)
    {
        //cout<<"Joining thread..."<<endl;
        thread.join();
    }

    //cout<<"Joined all threads."<<endl;
}

void ThreadPool::schedule(shared_ptr<IRunner> runner)
{
    lock_guard<mutex> lock(m_);
    queue_.push_back(runner);

    cv_.notify_one();
}

void ThreadPool::threadLoop()
{
    //cout<<"Thread started."<<endl;
    do
    {
        if(shutDown_)
            return;

        shared_ptr<IRunner> runner = pop();

        //cout<<"Signal!"<<endl;

        if(runner)
            runner->run();
    }
    while(!shutDown_);

    //cout<<"Thread stopped."<<endl;
}

shared_ptr<IRunner> ThreadPool::pop()
{
    unique_lock<mutex> lock(m_);

    while(queue_.empty() && !shutDown_)
       cv_.wait(lock);

    if(!queue_.empty())
    {
        shared_ptr<IRunner> result = queue_.back();
        queue_.pop_back();
        return result;
    }
    else
        return shared_ptr<IRunner>();
}

void ThreadPool::join()
{
    while(true)
    {
        {
            unique_lock<mutex> lock(m_);
            if(queue_.empty())
                return;
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}
