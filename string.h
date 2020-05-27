#ifndef STRING_H
#define STRING_H

#include <cstdio>
#include <iostream>

class String {
    char* buffer_;
    size_t size_;
    size_t capacity_;

    const static size_t kIncreaseFactor = 2;

public:
    String();
    explicit String(const char* str);
    explicit String(size_t size, char symbol = 'a');
    String(const char* str, size_t n);
    String(const String& other);

    ~String();

    size_t Size() const;
    size_t Length() const;
    size_t Capacity() const;

    bool Empty() const;

    void Resize();
    void Resize(size_t new_size, char fill = '\0');

    void Reserve(size_t new_capacity);

    void Clear();

    void PushBack(char symbol);
    void PopBack();

    char& Back();
    const char& Back() const;

    char& Front();
    const char& Front() const;

    const char* CStr() const;
    const char* Data() const;

    const char& operator[](size_t idx) const;
    char& operator[](size_t idx);

    String& operator+=(const String& other);
    String& operator+=(char symbol);

    String& operator=(const String& other);

    friend std::istream& operator>>(std::istream &is, String& str);
    friend std::ostream& operator<<(std::ostream &os, String& str);
};

String& operator+(String& lhs, String& rhs);

bool operator<(String& lhs, String& rhs);
bool operator>(String& lhs, String& rhs);
bool operator>=(String& lhs, String& rhs);
bool operator<=(String& lhs, String& rhs);

bool operator==(String& lhs, String& rhs);
bool operator!=(String& lhs, String& rhs);

std::istream& operator>>(std::istream &is, String& str);
std::ostream& operator<<(std::ostream &os, String& str);

#endif //STRING_H

