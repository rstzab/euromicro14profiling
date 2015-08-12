#ifndef USAGEREPORTER_H
#define USAGEREPORTER_H

#include <mutex>
#include <thread>


#include "../utils/platformDependent.h"

using namespace std;

class UsageReporter
{
    public:
        //typedef void UsageProcessor(long usedMemory);

        UsageReporter(function<void(const string&, long)> processor=&reportUsage, long reportIntervalMs = 1000);
        ~UsageReporter();

        void measure(const string& reason);

    protected:
    private:

        // private methods
        UsageReporter(const UsageReporter& other);
        void run();
        static void reportUsage(const string& reason, long usedMemory);

        // private fields
        bool volatile enough;
        long initalMemoryUsage;
        chrono::milliseconds reportInterval;
        thread myThread;
        //mutex m_;
        function<void(const string&, long)> usageProcessor;
};

#endif // USAGEREPORTER_H
