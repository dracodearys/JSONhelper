#ifndef _UNCOPYABLE_H_
#define _UNCOPYABLE_H_

class uncopyable {
public:
    uncopyable() = default;
    ~uncopyable() = default;
    uncopyable& operator= (const uncopyable&) = default;
    uncopyable(const uncopyable&) = delete;
};

#endif