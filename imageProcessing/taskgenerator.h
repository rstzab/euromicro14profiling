#ifndef TASKGENERATOR_H
#define TASKGENERATOR_H

#include <vector>
#include <string>
#include "task.h"

class TaskGenerator
{
    public:
        static std::vector<Task> generateTasks(std::string file, size_t step = 1000);
        static std::vector<Task> generateTasks(std::vector<std::string> files, size_t step = 1000);

        static void printTasks(std::vector<Task> tasks);
    protected:
    private:
};

#endif // TASKGENERATOR_H
