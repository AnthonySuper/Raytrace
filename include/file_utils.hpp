#pragma once
#include <stdexcept>
#if defined(unix) || defined(__APPLE__)
#include <sys/stat.h>
#endif

namespace NM {
    namespace FileUtils {
        void mkdir_p(const char *path);
    }
}
