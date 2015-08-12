#include "usagecsvsaver.h"

using namespace std;

std::shared_ptr<UsageCSVSaver> UsageCSVSaver::state_ (new UsageCSVSaver());

void UsageCSVSaver::start()
{
    if(state_->ofs_.is_open())
        state_->ofs_.close();

    state_->ofs_.open(state_->filename_, std::ofstream::out | std::ofstream::app);

    state_->start_ = std::chrono::steady_clock::now();
}

void UsageCSVSaver::save(const string& reason, long measurement)
{
    size_t timeElapsed = chrono::duration_cast<chrono::microseconds>
                                    (chrono::steady_clock::now() - start_).count();

    {
        lock_guard<mutex> lock(m_);
        ofs_ << reason <<", "<<timeElapsed<<", "<<measurement<<endl;
    }
}
