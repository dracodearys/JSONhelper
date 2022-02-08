#ifndef _JSONVALUE_H_
#define _JSONVALUE_H_

#include <utility>
#include "json.h"
#include "jsonException.h"

namespace json{

// Json类中unique_ptr<JsonValue> __value
class JsonValue{
public:
    virtual ~JsonValue() = default;

    virtual JsonType type() const = 0;   // pure virtual

    virtual bool toBool() const {
        throw JsonException("not a bool");
    }
    virtual double toDouble() const {
        throw JsonException("not a number");
    }
    virtual const std::string& toString() const {
        throw JsonException("not a string");
    }
    virtual const Json::array_t& toArray() const {
        throw JsonException("not an array");
    }
    virtual const Json::object_t& toObject() const {
        throw JsonException("not an object");
    }
    // 重载array[]
    virtual Json& operator[](size_t) {
        throw JsonException("not an array");
    }
    virtual const Json& operator[](size_t) const {
        throw JsonException("not an array");
    }
    // 重载object[]
    virtual Json& operator[](const std::string&) {
        throw JsonException("not an object");
    }
    virtual const Json& operator[](const std::string&) const {
        throw JsonException("not an object");
    }

    virtual size_t size() const noexcept{ return 0; }
};

template <typename T, JsonType U>
class Value : public JsonValue {
public:
    Value(const T& val) : _val(val) {}
    Value(T&& val) : _val(val) {}

    JsonType type() const final {
        return U;
    };  // 这加个分号，自动语法就对了，不加就不对
protected:
    T _val;
};


class JsonNull final : public Value<std::nullptr_t, JsonType::tNULL>{
public:
    explicit JsonNull(std::nullptr_t) : Value(nullptr) {}
};

class JsonBool final : public Value<bool, JsonType::tBOOL> {
public:
    explicit JsonBool(bool val) : Value(val) {}
    bool toBool() const override {
        return _val;
    }
};

class JsonDouble final : public Value<double, JsonType::tNUM>{
public:
    explicit JsonDouble(double val) : Value(val) {}
    double toDouble() const override {
        return _val;
    }
};

class JsonString final : public Value<std::string, JsonType::tSTR>{
public:
    explicit JsonString(const std::string& val) : Value(val) {}
    explicit JsonString(std::string&& val) : Value(std::move(val)){}
    const std::string& toString() const override {
        return _val;
    }
};

class JsonArray final : public Value<Json::array_t, JsonType::tARRAY>{
public:
    explicit JsonArray(const Json::array_t& val) : Value(val) {}
    explicit JsonArray(Json::array_t&& val) : Value(val) {}
    const Json::array_t& toArray() const override {
        return _val;
    }
    const Json& operator[](size_t i) const override {
        return _val[i];
    }
    Json& operator[](size_t i) override{
        return _val[i];
    }
    size_t size() const noexcept override {
        return _val.size();
    }
};

class JsonObject final : public Value<Json::object_t, JsonType::tOBJ>{
public:
    explicit JsonObject(const Json::object_t& val) : Value(val) {}
    explicit JsonObject(Json::object_t&& val) : Value(std::move(val)) {}
    const Json::object_t& toObject() const override{
        return _val;
    }
    const Json& operator[](const std::string& i) const override {
        return _val.at(i);
    }
    Json& operator[](const std::string& i) override {
        return _val.at(i);
    }
    size_t size() const noexcept override{
        return _val.size();
    }

};

}   // namespace json

#endif