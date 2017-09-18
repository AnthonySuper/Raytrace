#pragma once
#include <stdexcept>
#include <string>

namespace NM {
    class ParseError : std::runtime_error {
    public:
        ParseError();
        ParseError(std::string what);
    };
    
    class FileNotFoundError : std::runtime_error {
    public:
        FileNotFoundError();
        FileNotFoundError(std::string fname);
    };
    
    class InvalidFilenameError : std::runtime_error {
    public:
        InvalidFilenameError();
        InvalidFilenameError(std::string fname);
    };
    
    class InvalidIntersectionError : std::runtime_error {
    public:
        InvalidIntersectionError();
    };
}
