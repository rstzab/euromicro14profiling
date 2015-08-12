#ifndef MEMORYPROFILER_H
#define MEMORYPROFILER_H

#include <string>
#include <vector>

#include "../utils/platformDependent.h"

class MemoryProfiler
{
    public:
        MemoryProfiler(const std::string& filename):filename_(filename), baseMemory_(current_used_memory()){};
        ~MemoryProfiler();

        void appendValue(const std::string& reason, long value) const;

        static std::vector<long> readProfile(const std::string& filename);

    protected:
    private:
        std::string filename_;
        long baseMemory_;
};

#endif // MEMORYPROFILER_H
