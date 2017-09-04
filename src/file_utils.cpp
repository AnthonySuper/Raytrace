#include <file_utils.hpp>

namespace NM {
    namespace FileUtils {
        void mkdir_p(const char *path) {
#if defined(unix) || defined(__APPLE__)
            mkdir(path, S_IRWXU);
#else
            throw std::runtime_error("mkdir_p not supported on this OS");
#endif
        }
    }
}
