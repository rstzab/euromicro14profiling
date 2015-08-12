#include "MeasureTime.h"

MeasureTime::MeasureTime(std::string reason):reason_(reason), start_(std::chrono::high_resolution_clock::now()),isReported_(false)
{
}

MeasureTime::~MeasureTime()
{
    if(!isReported_)
        report();
}

//MeasureTime::MeasureTime(const MeasureTime& other)
//{
//    //copy ctor
//}


void MeasureTime::report()
{
    auto end = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start_);
    isReported_ = true;
    std::cout <<"MEASURE: "<<reason_<< " took " << dur.count() << "ms" << std::endl;
}
