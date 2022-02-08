#ifndef _JSON_H_
#define _JSON_H_

#include <iosfwd>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace json {

class JsonValue;

enum JsonType{
    tNULL,
    tBOOL,
    tNUM,
    tSTR,
    tARRAY,
    tOBJ
};

class Json final {
public:
    // define alias
    using array_t = std::vector<Json>;
    using object_t = std::unordered_map<std::string, Json>;

    // parse string to json
    // if error happens, errmsg storage the error msg.
    static Json parse(const std::string& content, std::string& errmsg) noexcept;
    // serialize json to string
    // 实现serialize，根据__value类型转换
    std::string serialize() const noexcept;

    // ctor
    explicit Json(std::nullptr_t);
    explicit Json(bool);
    explicit Json(double);
    explicit Json(int val) : Json(1.0 * val) {};
    explicit Json(const std::string&);
    // special ctor for C-style string
    // without this ctor, ' Json("xxxx") ' call Json(bool). 
    explicit Json(const char* cstr) : Json(std::string(cstr)) {};
    explicit Json(const array_t&);
    explicit Json(const object_t&);
    Json(const Json&);
    Json(Json&&) noexcept;

    // copy op=
    Json& operator=(Json);

    //dtor
    ~Json();

    // Accesses the type of JSON value the current value instance is
    JsonType type() const noexcept;
    // Is the current value a null value?
    bool isNull() const noexcept;
    // Is the current value a boolean value?
    bool isBool() const noexcept;
    // Is the current value a number value?
    bool isNumber() const noexcept;
    // Is the current value a string value?
    bool isString() const noexcept;
    // Is the current value a array value?
    bool isArray() const noexcept;
    // Is the current value a object value?
    bool isObject() const noexcept;

    // Converts the JSON value to a C++ boolean, if and only if it is a boolean
    bool toBool() const;
    // Converts the JSON value to a C++ double, if and only if it is a double
    double toDouble() const;
    // Converts the JSON value to a C++ string, if and only if it is a string
    const std::string& toString() const;
    // Converts the JSON value to a json array, if and only if it is an array
    const array_t& toArray() const;
    // Converts the JSON value to a json object, if and only if it is an object
    const object_t& toObject() const;

    // Accesses a field of a JSON array
    Json& operator[](std::size_t);
    // Accesses a field of a JSON array
    const Json& operator[](std::size_t) const;
    // Accesses a field of a JSON object
    Json& operator[](const std::string&);
    // Accesses a field of a JSON object
    const Json& operator[](const std::string&) const;

    /* 
        C++的编译器会在你的每个非静态的成员函数的参数列表中增加一个this指针，
    因为所有对象的代码在内存中就一份，this指针的功能是谁调用这个成员函数，
    该成员函数的这个this指针就指向谁，从而处理谁的数据。带不带const的成员函数
    的this指针是不同的：
        const的成员函数的this指针是(*const A*)类型的；不带const的成员函数的this指针是(A*)类型的。
    */
    /*
        考虑到const修饰的对象的函数调用，也就是说必须做两套成员函数，
    一套给有const修饰的对象（比如常量字符串）用，一套给没有const修饰的对象用。对于const
    的该操作符重载函数其返回值也应该是const的，否则就会出现可以通过其修改const对象的漏洞。

    例如： 为了给String提供一个[]操作符来读写指定位置的字符，需要提供如下两个函数，以分别对非const String对象和const String对象提供支持：
    char& operator[](int posion) { return data[posion]; };
    const char& operator[](int posion) const { return data[posion]; }
    */

    std::size_t size() const noexcept;

private:
    // copy-and-swap idiom 特别针对copy-assignment operator的实现
    void swap(Json&) noexcept; 
    // only used internally by serialize() 
    std::string serializeString() const noexcept;
    std::string serializeArray() const noexcept;
    std::string serializeObject() const noexcept;

    std::unique_ptr<JsonValue> __value;
};

// io func 
// 要在头文件中定义函数的话，需要加inline，否则其他文件包含多次该头文件就出现“重复定义”错误。
inline std::ostream& operator<< (std::ostream& os, const Json& json){
    return os << json.serialize();
}

// compare func
bool operator== (const Json&, const Json&) noexcept;
inline bool operator!= (const Json& lhs, const Json& rhs) noexcept{
    return !(lhs == rhs);
}

}   // namespace json



#endif 