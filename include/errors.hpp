#pragma once
#include <stdexcept>
#include <string>

namespace NM {
    class ParseError : public std::runtime_error {
    public:
        ParseError();
        ParseError(std::string what);
    };
    
    class FileNotFoundError : public std::runtime_error {
    public:
        FileNotFoundError();
        FileNotFoundError(std::string fname);
    };
    
    class InvalidFilenameError : public std::runtime_error {
    public:
        InvalidFilenameError();
        InvalidFilenameError(std::string fname);
    };
    
    class InvalidIntersectionError : public std::runtime_error {
    public:
        InvalidIntersectionError();
    };
}
