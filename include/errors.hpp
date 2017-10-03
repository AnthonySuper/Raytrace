#pragma once
#include <stdexcept>
#include <string>

namespace NM {
    /**
     * @brief An error thrown when parsing fails.
     */
    class ParseError : std::runtime_error {
    public:
        ParseError();
        ParseError(std::string what);
    };
    
    /**
     * @brief An error thrown when a file cannot be open.
     */
    class FileNotFoundError : std::runtime_error {
    public:
        FileNotFoundError();
        FileNotFoundError(std::string fname);
    };
    
    /**
     * @brief An error thrown when the name of a file is invalid/
     */
    class InvalidFilenameError : std::runtime_error {
    public:
        InvalidFilenameError();
        InvalidFilenameError(std::string fname);
    };
    
    /**
     * @brief An error thrown when you dereference an invalid intersection
     */
    class InvalidIntersectionError : std::runtime_error {
    public:
        InvalidIntersectionError();
    };
}
