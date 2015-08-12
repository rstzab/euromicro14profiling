#include <iostream>
#include <fstream>
#include <cstdio>

#include "usageReporter/UsageReporter.h"
#include "imageProcessing/taskgenerator.h"
#include "utils/threadpool.h"
#include "imageProcessing/taskprocessor.h"
#include "scheduler/memoryprofiler.h"
#include "scheduler/memorybasedexecutioncontroller.h"
#include "usageReporter/usagecsvsaver.h"

using namespace std;

void generateProfile(const Task& task, const string& profilePath, const string& usageReporterPath)
{
    UsageCSVSaver::setFilename(usageReporterPath);
    MemoryProfiler profiler(profilePath);
    cout<<endl;
    UsageReporter timeReporter([&](const string& reason, long measurement){
                                UsageCSVSaver::appendMeasurement(reason, measurement);
                                profiler.appendValue(reason, measurement);
                            }, 0);
    {
        UsageCSVSaver::start();
        TaskProcessor(task, &timeReporter).run();
        UsageCSVSaver::stop();
    }
}

void generateBaseline(const vector<Task>& tasks, const string& usageReporterPath, size_t threadCount)
{
    UsageCSVSaver::setFilename(usageReporterPath);
    UsageReporter timeReporter(&UsageCSVSaver::appendMeasurement, 0);

    {
        ThreadPool pool(threadCount);

        UsageCSVSaver::start();

        for(auto& task : tasks)
            pool.schedule(make_shared<TaskProcessor>(task, &timeReporter));

        pool.join();
    }
    UsageCSVSaver::stop();
}

void generateScheduledExecution(const vector<Task>& tasks, const string& usageReporterPath, const string& profilePath,
                                            size_t threadCount, long memoryLimit){

    UsageCSVSaver::setFilename(usageReporterPath);
    MemoryBasedExecutionController controller(MemoryProfiler::readProfile(profilePath), memoryLimit);

    UsageReporter timeReporter([&](const string& reason, long measurement){
                                UsageCSVSaver::appendMeasurement(reason, measurement);
                                controller.onNextMeasurement(reason, measurement);}, 0);

    {
        ThreadPool pool(threadCount);

        UsageCSVSaver::start();

        for(auto& task : tasks)
            pool.schedule(make_shared<TaskProcessor>(task, &timeReporter));

        pool.join();
    }
    UsageCSVSaver::stop();
}

int main()
{
    //
    // Application Configuration.
    //
    const vector<string> INPUT_FILES{"data/ev050HR_3D.png", "data/ev051HR_3D.png", "data/ev059HR_3D.png"}; //  files to process
    const size_t TASK_STEP = 2000; // size of subimage to assign for each task, in pixcels.
    const string MEMORY_PROFILE = "mprofile.prf"; //where memory profile will be stored and loaded from.
    const string SINGLE_TASK_CSV = "01_singleTask.csv";// Here is stored csv of single task execution.
    const string BASELINE_CSV = "02_baseline.csv";// Here is the baseline: uncontrolled execution of all tasks.
    const string SCHEDULED_CSV = "03_scheduled.csv";// Here is scheduled execution result.
    const size_t USE_THREADS = 4; // amount of threads to use for task execution.
	//
	// current memory usage limit for scheduled execution. 
	// This value should be slightly less (around 20% - 50MB) than actual resource usage boundary 
	// because of memory usage by other parts of application (thread pool, scheduler itself, results saver...)
	//
	const long MEMORY_LIMIT_B = 250*1024*1024;

    //
    // Cleanup.
    //
    cout<<"Cleaning up environment... ";

    remove(MEMORY_PROFILE.c_str());
    remove(SINGLE_TASK_CSV.c_str());
    remove(BASELINE_CSV.c_str());
    remove(SCHEDULED_CSV.c_str());

    cout<<"DONE."<<endl;

    //
    // Generating Tasks.
    //
    cout<<"Generating tasks..."<<endl;
    vector<Task> tasks = TaskGenerator::generateTasks(INPUT_FILES, TASK_STEP);
    cout<<"Generated "<<tasks.size()<<" tasks."<<endl;

    if(tasks.size() == 0)
    {
        cout<<"ERROR! No tasks generated. cannot continue."<<endl;
        return -1;
    }
    //
    // Generating Profile.
    //
    cout<<"Generating profile... "<<endl;
    generateProfile(tasks.at(0),MEMORY_PROFILE, SINGLE_TASK_CSV);

    //
    // Executing tasks uncontrolled, for baseline.
    //
    cout<<"Generating baseline (will take few minutes)... "<<endl;
    generateBaseline(tasks, BASELINE_CSV, USE_THREADS);

    //
    // Executing scheduled tasks.
    //
    cout<<"Generating scheduled execution (will take few minutes)... "<<endl;
    generateScheduledExecution(tasks, SCHEDULED_CSV, MEMORY_PROFILE, USE_THREADS, MEMORY_LIMIT_B);

    cout<<"Executing finished."<<endl;
}
