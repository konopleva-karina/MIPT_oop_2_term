#include "string.h"

String::String(): size_(0), capacity_(1) {
    buffer_ = new char[1];
    buffer_[0] = '\0';
}

String::String(size_t size, char symbol): size_(size), capacity_(size) {
    buffer_ = new char[Size() + 1];

    for (int i = 0; i < Size(); ++i) {
        buffer_[i] = symbol;
    }

    buffer_[Size()] = '\0';
}

size_t Size(const char* str) {
    size_t str_size = 0;
    while (str[str_size] != '\0'){
        ++str_size;
    }
    return str_size;
}

String::String(const char* str) {
    buffer_ = new char[Size() + 1];

    size_t str_size = ::Size(str);

    for (size_t i = 0; i < str_size; ++i) {
        buffer_[i] = str[i];
    }

    size_ = str_size;
    capacity_ = str_size;
    buffer_[Size()] = '\0';
}

String::String(const char* str, const size_t size): size_(size), capacity_(size) {
    buffer_ = new char[size + 1];

    for (size_t i = 0; i < size; ++i) {
        buffer_[i] = str[i];
    }

    buffer_[size] = '\0';
}

String::String(const String& other): size_(other.size_), capacity_(other.capacity_){
    buffer_ = new char[Size() + 1];
    for (int i = 0; i < Size() + 1; ++i) {
        buffer_[i] = other.buffer_[i];
    }
}

void String::Resize(){
    Resize(kIncreaseFactor * size_);
}

void String::Resize(size_t new_size, char fill) {
    char* new_str = new char[new_size + 1];

    for(int i = 0; i < Size() && i < new_size; ++i) {
        new_str[i] = buffer_[i];
    }

    for (int i = Size(); i < new_size; ++i) {
        new_str[i] = fill;
    }

    new_str[new_size] = '\0';

    delete[] buffer_;

    capacity_ = new_size;
    buffer_ = new_str;
}


String& String::operator=(const String& other) {
    if (this == &other) {
        return *this;
    }

    if (Size() < other.size_) {
        Resize(other.size_);
    }

    for (int i = 0; i < other.Size() + 1; ++i) {
        buffer_[i] = other.buffer_[i];
    }

    size_ = other.Size();
}

size_t String::Size() const {
    return size_;
}

size_t String::Length() const {
    return size_;
}

size_t String::Capacity() const {
    return capacity_;
}

bool String::Empty() const {
    return Size() == 0;
}

void String::Reserve(size_t new_capacity) {
    if (new_capacity > Capacity()) {
        Resize(new_capacity);
    }
}

void String::Clear() {
    size_ = 0;
    buffer_[0] = '\0';
}

char& String::Back() {
    return buffer_[Size() - 1];
}

char& String::Front() {
    return buffer_[0];
}

const char& String::Back() const {
    return buffer_[Size() - 1];
}

const char& String::Front() const {
    return *buffer_;
}

String::~String() {
    delete[] buffer_;
}

char& String::operator[](size_t idx){
    return buffer_[idx];
}

const char& String::operator[](size_t idx) const {
    return buffer_[idx];
}

void String::PushBack(char symbol) {
    if (Size() >= Capacity()) {
        Resize();
    }

    ++size_;

    buffer_[Size() - 1] = symbol;
    buffer_[Size()] = '\0';

}

void String::PopBack() {
    if (!Empty()) {
        buffer_[Size() - 1] = '\0';
        --size_;
    }
}

const char* String::CStr() const {
    return buffer_;
}

const char* String::Data() const {
    return buffer_;
}

String& String::operator+=(const String& other) {
    for (size_t i = 0; i < other.Size(); ++i) {
        PushBack(other.buffer_[i]);
    }

    buffer_[Size()] = '\0';

    return *this;
}

String& String::operator+=(char symbol) {
    PushBack(symbol);
    return *this;
}

String& operator+(String& lhs, String& rhs) {
    String* concatenated_str = new String;
    *concatenated_str += lhs;
    *concatenated_str += rhs;

    return *concatenated_str;
}

bool operator<(String& lhs, String& rhs) {
    for (size_t i = 0; i < lhs.Size() && rhs.Size(); ++i) {
        if (lhs[i] > rhs[i]) {
            return false;
        }
    }

    return lhs.Size() <= lhs.Size();
}

bool operator==(String& lhs, String& rhs) {
    if (lhs.Size() != rhs.Size()) {
        return false;
    }

    for (size_t i = 0; i < lhs.Size(); ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }

    return true;
}

bool operator>(String& lhs, String& rhs) {
    return !(lhs < rhs || lhs == rhs);
}

bool operator>=(String& lhs, String& rhs) {
    return lhs > rhs || lhs == rhs;
}

bool operator<=(String& lhs, String& rhs) {
    return lhs < rhs || lhs == rhs;
}

bool operator!=(String& lhs, String& rhs) {
    return !(lhs == rhs);
}

std::istream& operator>>(std::istream &is, String& str) {
    char word = '0';

    word = static_cast<char>(getchar());
    while(!isspace(word)) {
        str += word;
        word = static_cast<char>(getchar());
    }

    return is;
}

std::ostream& operator<<(std::ostream &os, String& str) {
    os << str.Data();
    return os;
}
