#include <errors.hpp>

namespace NM {
    
    TracerError::TracerError(const std::string &w) :
        std::runtime_error(w)
    {}
    
    ParseError::ParseError() :
        TracerError("Error parsing something")
    {}
    
    ParseError::ParseError(std::string what) :
        TracerError(what)
    {}
    
    FileNotFoundError::FileNotFoundError() :
        TracerError("File not found") {}
    
    FileNotFoundError::FileNotFoundError(std::string fname) :
    TracerError("File `" + fname + "` not found") {}
    
    
    InvalidFilenameError::InvalidFilenameError() :
    TracerError("Filename is invalidly formatted") {}
    
    InvalidFilenameError::InvalidFilenameError(std::string fname) :
    TracerError("Filename `" + fname + "` not found") {}
    
    InvalidIntersectionError::InvalidIntersectionError() :
    TracerError("Tried to perform an invalid operation on a non-existant intersection")
    {}
    
    
    
    
}
