#ifndef TASK_H
#define TASK_H

#include <string>

class Task
{

    public:
        Task(std::string filePath, size_t pos_x, size_t pos_y, size_t size_x, size_t size_y):
            filePath(filePath), pos_x(pos_x), pos_y(pos_y), size_x(size_x), size_y(size_y),
            task_id(Next_Task_Id++)
            {}
        ~Task(){};

        std::string GetfilePath() const { return filePath; }
        void SetfilePath(std::string val) { filePath = val; }

        size_t Getpos_x() const { return pos_x; }
        void Setpos_x(size_t val) { pos_x = val; }

        size_t Getpos_y() const { return pos_y; }
        void Setpos_y(size_t val) { pos_y = val; }

        size_t Getsize_x() const { return size_x; }
        void Setsize_x(size_t val) { size_x = val; }

        size_t Getsize_y() const { return size_y; }
        void Setsize_y(size_t val) { size_y = val; }

        size_t Gettask_id() const { return task_id; }

    protected:
    private:
        std::string filePath;
        size_t pos_x;
        size_t pos_y;
        size_t size_x;
        size_t size_y;
        size_t task_id;
        static size_t Next_Task_Id;
};

#endif // TASK_H
