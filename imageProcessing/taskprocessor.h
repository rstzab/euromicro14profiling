#ifndef TASKPROCESSOR_H
#define TASKPROCESSOR_H
#include "../usageReporter/UsageReporter.h"
#include "../utils/threadpool.h"
#include "task.h"

class TaskProcessor : public IRunner
{
    public:
        TaskProcessor(const Task& task, UsageReporter* usageReporter):reportId_(0),task_(task),reporter_(usageReporter) {}
        virtual ~TaskProcessor(){}
        void run();
    protected:
    private:

        void report(const string& reason);
        size_t reportId_;

        Task task_;
        UsageReporter* reporter_;
};

#endif // TASKPROCESSOR_H
