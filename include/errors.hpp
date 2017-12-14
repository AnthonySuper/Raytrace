#pragma once
#include <stdexcept>
#include <string>

namespace NM {
    
    class TracerError : public std::runtime_error {
    public:
        TracerError(const std::string &);
    };
    /**
     * @brief An error thrown when parsing fails.
     */
    class ParseError : public TracerError {
    public:
        ParseError();
        ParseError(std::string what);
    };
    
    /**
     * @brief An error thrown when a file cannot be open.
     */
    class FileNotFoundError : TracerError {
    public:
        FileNotFoundError();
        FileNotFoundError(std::string fname);
    };
    
    /**
     * @brief An error thrown when the name of a file is invalid/
     */
    class InvalidFilenameError : public TracerError {
    public:
        InvalidFilenameError();
        InvalidFilenameError(std::string fname);
    };
    
    /**
     * @brief An error thrown when you dereference an invalid intersection
     */
    class InvalidIntersectionError : public TracerError {
    public:
        InvalidIntersectionError();
    };
}
