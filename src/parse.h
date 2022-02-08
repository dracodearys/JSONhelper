#ifndef _PARSE_H_
#define _PARSE_H_

#include "json.h"
#include "jsonException.h"
#include "uncopyable.h"

namespace json {

class Parser final : uncopyable {
public: 
    Parser(const std::string& content) noexcept : __start(content.c_str()), __cur(content.c_str()) {}
    Json parse();
private:
    Json parseValue();
    Json parseLiteral(const std::string& literal);
    Json parseNumber();
    std::string parseRawString();
    Json parseString() {
        return Json(parseRawString());
    };
    unsigned parse4hex();
    std::string encodeUTF8(unsigned u) noexcept;
    Json parseArray();
    Json parseObject();
    void parseWhitespace() noexcept;

    [[noreturn]] void error(const std::string& msg) const {
        throw JsonException(msg + ":" + __start);
    }

    const char* __start;
    const char* __cur;
};
    
}   // namespace json

#endif