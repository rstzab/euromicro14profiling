#ifndef MEMORYBASEDEXECUTIONCONTROLLER_H
#define MEMORYBASEDEXECUTIONCONTROLLER_H

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <unordered_map>

class MemoryBasedExecutionController
{
    public:
        MemoryBasedExecutionController(const std::vector<long>& memoryProfile, long memoryLimit):
        memory_limit_(memoryLimit), profile_(memoryProfile){}

        ~MemoryBasedExecutionController();

        void onNextMeasurement(const std::string& reason, long value);
    protected:
    private:
        bool parseReason(const std::string& reason, std::pair<size_t,size_t>& result);
        long calculateExpectedLoad();
        std::shared_ptr<std::condition_variable> findTaskToResume(long expectedLoad);

    long memory_limit_;
    std::vector<long> profile_;
    std::mutex m_;
    std::vector<std::pair<long, std::shared_ptr<std::condition_variable>>> blocked_threads_;
    std::unordered_map<size_t, long> expected_load_;
};

#endif // MEMORYBASEDEXECUTIONCONTROLLER_H
