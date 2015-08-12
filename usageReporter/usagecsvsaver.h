#ifndef USAGECSVSAVER_H
#define USAGECSVSAVER_H

#include <memory>
#include <string>
#include <mutex>
#include <chrono>
#include <fstream>

class UsageCSVSaver
{
    public:
        static void start();

        static void stop() {state_->ofs_.close();}

        static void setFilename(const std::string& filename){state_->filename_ = filename;}

        static void appendMeasurement(const std::string& reason, long measurement){state_->save(reason, measurement);}

        ~UsageCSVSaver(){};
        UsageCSVSaver(){};

    protected:
    private:
         void save(const std::string& reason, long measurement);

    static std::shared_ptr<UsageCSVSaver> state_;

    std::chrono::steady_clock::time_point start_;
    std::string filename_;
    std::mutex m_;
    std::ofstream ofs_;
};

#endif // USAGECSVSAVER_H
