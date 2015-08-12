#include "taskgenerator.h"
#include <iostream>

#include "CImg.h"

using namespace std;
using namespace cimg_library;

vector<Task> TaskGenerator::generateTasks(string file, size_t step)
{
    return generateTasks(vector<string>{file}, step);
}

vector<Task> TaskGenerator::generateTasks(vector<string> files, size_t step)
{
    vector<Task> result;

    for(auto& file : files)
    {
        CImg<unsigned char> image(file.c_str());

        size_t max_x = image.width();
        size_t max_y = image.height();
        size_t x=0,y=0;
        while(x < max_x && y < max_y)
        {
            size_t new_x = x+step;
            size_t new_y = y+step;

            if(new_x <= max_x && new_y <= max_y)
            {
                result.emplace_back(file, x, y, new_x - x, new_y - y);
            }

            x = new_x;

            if(x >= max_x)
            {
                x = 0;
                y = new_y;
            }
        }
    }

    return result;
}

void TaskGenerator::printTasks(vector<Task> tasks)
{
    for(auto& task : tasks)
    {
        cout<<task.GetfilePath()<<": "<<task.Getpos_x()<<" + "<<task.Getsize_x()
            <<", "<<task.Getpos_y()<<" + "<<task.Getsize_y()<<endl;
    }
    cout<<"Total: "<<tasks.size()<<" tasks."<<endl;
}
