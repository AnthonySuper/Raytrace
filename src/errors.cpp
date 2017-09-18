#include <errors.hpp>

namespace NM {
    
    ParseError::ParseError() :
        runtime_error("Error parsing something")
    {}
    
    ParseError::ParseError(std::string what) :
        std::runtime_error(what)
    {}
    
    FileNotFoundError::FileNotFoundError() :
        std::runtime_error("File not found") {}
    
    FileNotFoundError::FileNotFoundError(std::string fname) :
    std::runtime_error("File `" + fname + "` not found") {}
    
    
    InvalidFilenameError::InvalidFilenameError() :
    std::runtime_error("Filename is invalidly formatted") {}
    
    InvalidFilenameError::InvalidFilenameError(std::string fname) :
    std::runtime_error("Filename `" + fname + "` not found") {}
    
    InvalidIntersectionError::InvalidIntersectionError() :
    std::runtime_error("Tried to perform an invalid operation on a non-existant intersection")
    {}
    
    
    
    
}
