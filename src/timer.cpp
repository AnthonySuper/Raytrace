#include <timer.hpp>

namespace NM {
    Timer::Timer() noexcept :
        lastReport(Timer::sclock::now()) {}

    Timer Timer::timer;

    void Timer::report(std::string msg) {
        auto now = Timer::sclock::now();
        auto dur = (now - Timer::timer.lastReport);
        std::chrono::duration<float> fsec(dur);
        std::cout << msg << " in " << fsec.count() << "s" << std::endl;
        Timer::timer.lastReport = now;
    }
};
