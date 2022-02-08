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
    // ʵ��serialize������__value����ת��
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
        C++�ı������������ÿ���Ǿ�̬�ĳ�Ա�����Ĳ����б�������һ��thisָ�룬
    ��Ϊ���ж���Ĵ������ڴ��о�һ�ݣ�thisָ��Ĺ�����˭���������Ա������
    �ó�Ա���������thisָ���ָ��˭���Ӷ�����˭�����ݡ�������const�ĳ�Ա����
    ��thisָ���ǲ�ͬ�ģ�
        const�ĳ�Ա������thisָ����(*const A*)���͵ģ�����const�ĳ�Ա������thisָ����(A*)���͵ġ�
    */
    /*
        ���ǵ�const���εĶ���ĺ������ã�Ҳ����˵���������׳�Ա������
    һ�׸���const���εĶ��󣨱��糣���ַ������ã�һ�׸�û��const���εĶ����á�����const
    �ĸò��������غ����䷵��ֵҲӦ����const�ģ�����ͻ���ֿ���ͨ�����޸�const�����©����

    ���磺 Ϊ�˸�String�ṩһ��[]����������дָ��λ�õ��ַ�����Ҫ�ṩ���������������Էֱ�Է�const String�����const String�����ṩ֧�֣�
    char& operator[](int posion) { return data[posion]; };
    const char& operator[](int posion) const { return data[posion]; }
    */

    std::size_t size() const noexcept;

private:
    // copy-and-swap idiom �ر����copy-assignment operator��ʵ��
    void swap(Json&) noexcept; 
    // only used internally by serialize() 
    std::string serializeString() const noexcept;
    std::string serializeArray() const noexcept;
    std::string serializeObject() const noexcept;

    std::unique_ptr<JsonValue> __value;
};

// io func 
// Ҫ��ͷ�ļ��ж��庯���Ļ�����Ҫ��inline�����������ļ�������θ�ͷ�ļ��ͳ��֡��ظ����塱����
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