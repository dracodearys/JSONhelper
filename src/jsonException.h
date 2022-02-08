#ifndef _JSONEXCEPTION_H_
#define _JSONEXCEPTION_H_

#include <stdexcept>
namespace json{
    
    class JsonException final : public std::runtime_error{
    public:
        explicit JsonException(const std::string& errmsg) : runtime_error(errmsg) {}
        const char* what() const noexcept override{
            return runtime_error::what(); // return val : A pointer to a c-string with content related to the exception.
        }
    };

}   // namespace json

#endif