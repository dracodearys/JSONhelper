#include "parse.h"
#include <cassert>
#include <cmath>        //HUGE_VAL
#include <cstdlib>      //strtod
#include <cstring>      //strncmp
#include <stdexcept>    //runtime_error

using namespace std;

namespace json{
constexpr bool is1to9(char ch) { return ch >= '1' && ch <= '9'; }
constexpr bool is0to9(char ch) { return ch >= '0' && ch <= '9'; }

Json Parser::parse() {
    parseWhitespace();
    Json json = parseValue();
    parseWhitespace();
    if (*__cur) error("ROOT NOT SINGULAR");
    return json;
}

Json Parser::parseValue(){
    switch (*__cur){
        case 'n': return parseLiteral("null");
        case 't': return parseLiteral("true");
        case 'f': return parseLiteral("false");
        case '\"': return parseString();    //直接调用parseRawString()
        case '[': return parseArray();
        case '{': return parseObject();
        case '\0': error("EXPECT VALUE");
        default: return parseNumber();
    }
}

Json Parser::parseLiteral(const string& literal) {
    if (strncmp(__cur, literal.c_str(), literal.size())) error("INVALID VALUE");
    __cur += literal.size();
    __start = __cur;
    switch (literal[0]) {
        case 't': return Json(true);
        case 'f': return Json(false);
        default: return Json(nullptr);
    }
}
Json Parser::parseNumber(){
    if (*__cur == '-') ++__cur;
    //int
    if (*__cur == '0') ++__cur;
    else {
        if (!is1to9(*__cur)) error("INVALID VALUE");
        while (is0to9(*++__cur));
    }
    //frac
    if (*__cur == '.') {
        if (!is0to9(*++__cur)) error("INVALID VALUE");
        while (is0to9(*++__cur));
    }
    //exp
    if (toupper(*__cur) == 'E'){
        ++__cur;
        if (*__cur == '-' || *__cur == '+') ++__cur;
        if (!is0to9(*__cur)) error("INVALID VALUE");
        while (is0to9(*++__cur));
    }
    //
    double val = strtod(__start, nullptr);
    if (fabs(val) == HUGE_VAL) error("NUMBER TOO BIG");
    __start = __cur;
    return Json(val);
}
string Parser::parseRawString() {
    string str;
    while(1){
        switch (*++__cur){
            case '\"' : __start = ++__cur; return str;
            case '\0' : error("MISS QUOTATION MARK");
            default :{
                if (static_cast<unsigned char>(*__cur) < 0x20) error("INVALID STRING CHAR");
                str.push_back(*__cur);
                break;
            }
            case '\\' :
                switch (*++__cur){
                    case '\"' : str.push_back('\"'); break;
                    case '\\': str.push_back('\\'); break;
                    case '/': str.push_back('/'); break;
                    case 'b': str.push_back('\b'); break;
                    case 'f': str.push_back('\f'); break;
                    case 'n': str.push_back('\n'); break;
                    case 't': str.push_back('\t'); break;
                    case 'r': str.push_back('\r'); break;
                    case 'u': {
                        unsigned u1 = parse4hex();
                        if (u1 >= 0xd800 && u1 <= 0xdbff) { // high surrogate
                            if (*++__cur != '\\') error("INVALID UNICODE SURROGATE");
                            if (*++__cur != 'u') error("INVALID UNICODE SURROGATE");
                            unsigned u2 = parse4hex();  // low surrogate
                            if (u2 < 0xdc00 || u2 > 0xdfff) error("INVALID UNICODE SURROGATE");
                            u1 = (((u1 - 0xd800) << 10) | (u2 - 0xdc00)) + 0x10000;
                        }
                        str += encodeUTF8(u1);
                    } break;
                    default : error("INVALID STRING ESCAPE");
                }
                break;
        }
    }
}

unsigned Parser::parse4hex(){
    unsigned u = 0;
    for (int i = 0; i != 4; ++i){
        // now *__cur = "uXXXX...." ...
        unsigned ch = static_cast<unsigned>(toupper(*++__cur));
        u <<= 4;
        if (ch >= '0' && ch <= '9') u |= (ch - '0');
        else if (ch >= 'A' && ch <= 'F') u |= ch - 'A' + 10;
        else error("INVALID UNICODE HEX");
    }
    return u;
}

string Parser::encodeUTF8(unsigned u) noexcept {
    string utf8;
    if (u <= 0x7F) // 0111,1111
        utf8.push_back(static_cast<char>(u & 0xff));
    else if (u <= 0x7FF){
        utf8.push_back(static_cast<char>(0xc0 | ((u >> 6) & 0xff)));
        utf8.push_back(static_cast<char>(0x80 | (u & 0x3f)));
    } else if (u <= 0xFFFF) {
        utf8.push_back(static_cast<char>(0xe0 | ((u >> 12) & 0xff)));
        utf8.push_back(static_cast<char>(0x80 | ((u >> 6) & 0x3f)));
        utf8.push_back(static_cast<char>(0x80 | (u & 0x3f)));
    } else {
        assert(u <= 0x10FFFF);
        utf8.push_back(static_cast<char>(0xf0 | ((u >> 18) & 0xff)));
        utf8.push_back(static_cast<char>(0x80 | ((u >> 12) & 0x3f)));
        utf8.push_back(static_cast<char>(0x80 | ((u >> 6) & 0x3f)));
        utf8.push_back(static_cast<char>(0x80 | (u & 0x3f)));
    }
    return utf8;
}

Json Parser::parseArray(){
    Json::array_t arr;
    ++__cur; // skip '['
    parseWhitespace();
    if (*__cur == ']') {
        __start = ++__cur;
        return Json(arr);
    }
    while (1) {
        parseWhitespace();
        arr.push_back(parseValue());
        parseWhitespace();
        if (*__cur == ',') ++__cur;
        else if (*__cur == ']'){
            __start = ++__cur;
            return Json(arr);
        }else error("MISS COMMA OR SQUARE BRACKET");
    }
}

Json Parser::parseObject(){
    Json::object_t obj;
    ++__cur;
    parseWhitespace();
    if (*__cur == '}') {
        __start = ++__cur;
        return Json(obj);
    }
    while (1) {
        parseWhitespace();
        if (*__cur != '"') error("MISS KEY");
        string key = parseRawString();
        parseWhitespace();
        if (*__cur++ != ':') error("MISS COLON");
        parseWhitespace();
        Json val = parseValue();
        obj.insert({key, val});
        parseWhitespace();
        if (*__cur == ',') ++__cur;
        else if (*__cur == '}'){
            __start = ++__cur;
            return Json(obj);
        }else error("MISS COMMA OR CURLY BRACKET");
    }
}

void Parser::parseWhitespace() noexcept {
    while (*__cur == ' ' || *__cur == '\r' || *__cur == '\t' || *__cur == '\n') ++__cur;
    __start = __cur;
}
}   //namespace json