#pragma once
#include <globals.hpp>
#include <chrono>
#include <string>

namespace NM {
    class Timer {
        std::chrono::time_point<std::chrono::steady_clock> lastReport;

        Timer() noexcept;

    public:

        using sclock = std::chrono::steady_clock;
        static Timer timer;
        static void report(std::string msg);
    };
};
