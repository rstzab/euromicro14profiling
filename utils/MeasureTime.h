#ifndef MEASURETIME_H
#define MEASURETIME_H

#include <iostream>
#include <chrono>

class MeasureTime
{
    public:
        MeasureTime(std::string reason);
        ~MeasureTime();
        void report();

    protected:
    private:
        MeasureTime(const MeasureTime& other);

        std::string reason_;
        std::chrono::high_resolution_clock::time_point start_;
        bool isReported_;
};

#endif // MEASURETIME_H
