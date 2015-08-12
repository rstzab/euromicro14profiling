#include "memorybasedexecutioncontroller.h"

#include <iostream>
using namespace std;

MemoryBasedExecutionController::~MemoryBasedExecutionController()
{
    //dtor
}

void MemoryBasedExecutionController::onNextMeasurement(const string& reason, long value)
{
    //
    // Parsing input
    //
    pair<size_t, size_t> pair;
    if(!parseReason(reason, pair))
    {
        cout<<"Failed to convert "<<reason<<" to int."<<endl;
        return;//string format is incorrect.
    }

    size_t taskId = pair.first;
    size_t currentStep = pair.second;

    {
        unique_lock<mutex> lock(m_);

        if(currentStep >= profile_.size()-1)
        {   // removing task from current tasks.
            expected_load_.erase(taskId);

            cout<<"Task "<<taskId<<" FINISHED. "<<expected_load_.size()<<" TASKS STILL RUNNING."<<endl;

            //if there are some blocked threads, unblock one
            shared_ptr<condition_variable> taskToResume = findTaskToResume(value);
            if(taskToResume)
                taskToResume->notify_one();

            return;//this is the last element, nothing more to parse. TODO: unblock threads.
        }

        long change = profile_[currentStep+1] - profile_[currentStep];

        //
        // Deciding on action
        //
        if(change > 0)
        {// only when we expect increase of memory usage,
         // we decide whether we block our task or not.
            bool suspendProcess = false;
            long expectedLoad = max(value, calculateExpectedLoad());//max of what we see and expect.

            // desiding whether we should suspend our process or not.
            suspendProcess = expectedLoad + change >= memory_limit_;
            // our process is last running if its either not registered in expected_load_
            // and all processes are blocked or all except of this one are blocked.
            if(suspendProcess && (blocked_threads_.size() ==
                            ((expected_load_.find(taskId) == expected_load_.end())
                            ? expected_load_.size() : expected_load_.size()-1)))
            {
                cout<<"WANT TO SUSPEND LAST RUNNING TASK "<<taskId<<". T:"
                <<expected_load_.size()<<" B:"<<blocked_threads_.size()<<endl;

                suspendProcess = false;
            }

            if(suspendProcess)
            {//saving suspension to allow unblocking by some other thread.
                cout<<"Task "<<taskId<<" SUSPENDED."<<endl;
                // defining maximal amount of memory usage when we can resume execution.
                long max_memory_limit = memory_limit_ - change;
                shared_ptr<condition_variable> threadLock = make_shared<condition_variable>();
                blocked_threads_.emplace_back(max_memory_limit, threadLock);

                // in case we block ourselves at first step, we have to write down our current usage
                if(expected_load_.find(taskId) == expected_load_.end())
                    expected_load_.emplace(taskId, profile_[currentStep]);

                threadLock->wait(lock);

                cout<<"Task "<<taskId<<" RESUMED."<<endl;

                // removing information on our blocking state.
                for(auto iter = blocked_threads_.begin(); iter != blocked_threads_.end();++iter)
                    if(iter->second == threadLock)
                    {
                        blocked_threads_.erase(iter);
                        break;
                    }
            }
        }
        else
        {// if we are decreasing memory usage, we can unblock one of threads.

            long expectedLoad = max(value, calculateExpectedLoad());//max of what we see and expect.

            // finding most requiring process we can unpause.
            shared_ptr<condition_variable> taskToResume = findTaskToResume(expectedLoad);

            if(taskToResume)
                taskToResume->notify_one();///resuming task.
        }

        //
        // Updating expectations
        //
        long newValue = profile_[currentStep+1];
        auto element = expected_load_.find(taskId);
        if(element == expected_load_.end())
            expected_load_.emplace(taskId, newValue);
        else
            element->second = newValue;
    }
}

shared_ptr<condition_variable> MemoryBasedExecutionController::findTaskToResume(long expectedLoad)
{
    bool haveTaskToResume = false;
    long taskToResumeMemory = 0;//maximal ammount of memory needed to be able to run this task.
    shared_ptr<condition_variable> taskToResume;//200  89 90 > 50

    for(auto& task : blocked_threads_)
        if(task.first > expectedLoad)
            if(!haveTaskToResume || taskToResumeMemory > task.first)
            {
                haveTaskToResume = true;
                taskToResumeMemory = task.first;
                taskToResume = task.second;
            }

    return taskToResume;
}

long MemoryBasedExecutionController::calculateExpectedLoad()
{
    long res = 0;
    for(auto& elem : expected_load_)
    {
        res += elem.second;
    }
    return res;
}

bool MemoryBasedExecutionController::parseReason(const string& reason, pair<size_t,size_t>& result)
{
    size_t splitPos = reason.find(':');
    if(splitPos == string::npos)
        return false;

    try
    {
        result.first = stoi(reason.substr(0, splitPos));
        result.second = stoi(reason.substr(splitPos+1));
        return true;
    }catch(invalid_argument& ex)
    {
        return false;
    }
}
