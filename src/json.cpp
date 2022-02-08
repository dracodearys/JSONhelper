#include "json.h"
#include "jsonValue.h"
#include "parse.h"
#include <cassert>
using namespace std;

namespace json{

// ctor
Json::Json(nullptr_t) : __value(make_unique<JsonNull>(nullptr)) {}
Json::Json(bool val) : __value(make_unique<JsonBool>(val)) {}
Json::Json(double val) : __value(make_unique<JsonDouble>(val)) {}
Json::Json(const string& val) : __value(make_unique<JsonString>(val)) {}
Json::Json(const array_t& val) : __value(make_unique<JsonArray>(val)) {}
Json::Json(const object_t& val) : __value(make_unique<JsonObject>(val)) {}
Json::Json(const Json& rhs) {
    switch (rhs.type()){
        case JsonType::tNULL : __value = make_unique<JsonNull>(nullptr); break;
        case JsonType::tBOOL : __value = make_unique<JsonBool>(rhs.toBool()); break;
        case JsonType::tNUM : __value = make_unique<JsonDouble>(rhs.toDouble()); break;
        case JsonType::tSTR : __value = make_unique<JsonString>(rhs.toString()); break;
        case JsonType::tARRAY : __value = make_unique<JsonArray>(rhs.toArray()); break;
        case JsonType::tOBJ : __value = make_unique<JsonObject>(rhs.toObject()); break;
    }
}
Json::Json(Json&& rhs) noexcept : __value(std::move(rhs.__value)){
    rhs.__value = nullptr;
}

// dtor 
Json::~Json() {}

// copy op=
Json& Json::operator=(Json rhs){
    swap(rhs);
    return *this;
}
void Json::swap(Json& rhs) noexcept{
    // copy-and-swap idiom
    using std::swap;
    swap(__value, rhs.__value);
}

bool Json::toBool() const {
    // __value是unique_ptr<JsonValue>，这里是调用JsonValue::toBool()
    return __value->toBool();
}
double Json::toDouble() const {
    return __value->toDouble();
}
const string& Json::toString() const {
    return __value->toString();
}
const Json::array_t& Json::toArray() const {
    return __value->toArray();
}
const Json::object_t& Json::toObject() const {
    return __value->toObject();
}

JsonType Json::type() const noexcept { 
    // 调用JsonValue::type()
    return __value->type(); 
}
bool Json::isNull() const noexcept { return type() == JsonType::tNULL; }
bool Json::isBool() const noexcept { return type() == JsonType::tBOOL; }
bool Json::isNumber() const noexcept { return type() == JsonType::tNUM; }
bool Json::isString() const noexcept { return type() == JsonType::tSTR; }
bool Json::isArray() const noexcept { return type() == JsonType::tARRAY; }
bool Json::isObject() const noexcept { return type() == JsonType::tOBJ; }
// 都是在调用JsonValue中的op[]
Json& Json::operator[](size_t i) { return __value->operator[](i); }
const Json& Json::operator[](size_t i) const { return __value->operator[](i); }
Json& Json::operator[](const string& i) { return __value->operator[](i); }
const Json& Json::operator[](const string& i) const { return __value->operator[](i); }

size_t Json::size() const noexcept { return __value->size(); }

Json Json::parse(const string& content, string& errmsg) noexcept{
    try{
        Parser p(content);
        return p.parse();
    } catch (JsonException& err) {
        errmsg = err.what();
        return Json(nullptr);
    }
}

string Json::serialize() const noexcept{
    switch (__value->type()){
        case JsonType::tNULL : return "null";
        case JsonType::tBOOL : return __value->toBool() ? "true" : "false";
        case JsonType::tNUM : 
            char buf[32];
            snprintf(buf, sizeof(buf), "%.17g", __value->toDouble());
            return buf;
        case JsonType::tSTR : return serializeString();
        case JsonType::tARRAY : return serializeArray();
        case JsonType::tOBJ : return serializeObject(); 
    }
    assert(0);
}

string Json::serializeString() const noexcept{
    string ret = "\"";
    for (auto e : __value->toString()){
        switch (e) {
            case '\"' : ret += "\\\""; break;
            case '\\': ret += "\\\\"; break;
            case '\b': ret += "\\b"; break;
            case '\f': ret += "\\f"; break;
            case '\n': ret += "\\n"; break;
            case '\r': ret += "\\r"; break;
            case '\t': ret += "\\t"; break;
            default : 
                if (static_cast<unsigned char>(e) < 0x20) {
                    char buf[7];
                    sprintf(buf, "\\u%04X", e);
                    ret += buf;
                }else ret += e;
        }
    }
    ret += "\"";
    return ret;
}

string Json::serializeArray() const noexcept{
    string ret = "[";
    for (size_t i = 0; i < __value->size(); ++i){
        if (i > 0) ret += ",";
        ret += (*this)[i].serialize();
    }
    ret += "]";
    return ret;
}

string Json::serializeObject() const noexcept{
    string ret = "{";
    bool first = 1;
    for (const pair<string, Json>& p : __value->toObject()){
        if (first) first = 0;
        else ret += ", ";
        ret += "\"" + p.first + "\"";
        ret += ": ";
        ret += p.second.serialize();
    }
    return ret + " }";
}

bool operator==(const Json& lhs, const Json& rhs) noexcept {
    if (lhs.type() != rhs.type()) return false;
    switch (lhs.type()){
        case JsonType::tNULL: return true;
        case JsonType::tBOOL: return lhs.toBool() == rhs.toBool();
        case JsonType::tNUM: return lhs.toDouble() == rhs.toDouble();
        case JsonType::tSTR: return lhs.toString() == rhs.toString();
        case JsonType::tARRAY: return lhs.toArray() == rhs.toArray();
        case JsonType::tOBJ: return lhs.toObject() == rhs.toObject();
    }
    assert(0);
}

}   // namespace json