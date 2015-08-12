#include "memoryprofiler.h"
#include <fstream>
using namespace std;

MemoryProfiler::~MemoryProfiler()
{
    //dtor
}
void MemoryProfiler::appendValue(const string& reason, long value) const
{
    ofstream ofs;
    ofs.open (filename_, ofstream::out | fstream::app);
    ofs << value - baseMemory_ <<" ";
    ofs.close();
}

vector<long> MemoryProfiler::readProfile(const string& filename)
{
    vector<long> result;

    ifstream ifs;
    ifs.open(filename, ifstream::in);
    long value = 0;
    while(ifs>>value)
    {
        result.push_back(value);
    }

    return result;
}
