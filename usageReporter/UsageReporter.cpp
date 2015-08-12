#include <iostream>
#include "UsageReporter.h"

UsageReporter::UsageReporter(function<void(const string&, long)> processor, long reportIntervalMs):
    enough(false),
    initalMemoryUsage(0),
    //initalMemoryUsage(current_used_memory()),
    reportInterval(reportIntervalMs),
    myThread(&UsageReporter::run,this),
    usageProcessor(processor)
{
}

UsageReporter::~UsageReporter()
{
    enough = true;
    myThread.join();
}

//UsageReporter::UsageReporter(const UsageReporter& other)
//{
//    //copy ctor
//}


//-----------------------------------------------------------------

void UsageReporter::measure(const string& reason)
{
    //lock_guard<mutex> lock(m_);
    long usedMemory = current_used_memory();
    usageProcessor(reason, usedMemory - initalMemoryUsage);
}

void UsageReporter::run()
{
    if(reportInterval.count() == 0)
        return;

    while(!enough)
    {
        {
            //lock_guard<mutex> lock(m_);
            long usedMemory = current_used_memory();
            usageProcessor("timer", usedMemory - initalMemoryUsage);
        }
        this_thread::sleep_for(reportInterval);
    }
}

void UsageReporter::reportUsage(const string& reason, long usedMemory)
{
    cout<<"Event by "<<reason<<". Using memory = "<<usedMemory<<" B"<<endl;
}
