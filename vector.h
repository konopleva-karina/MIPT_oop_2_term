#ifndef VECTOR_H
#define VECTOR_H

#include <cstdlib>

template <class T>
class Vector {
    T* buffer_;
    size_t size_;
    size_t capacity_;

    const static size_t kIncreaseFactor = 2;

    void Fill(size_t start, size_t end, const T& value);
    size_t FindCorrectCapacity();
    void BufferReallocation(size_t new_capacity);

public:
    Vector();
    explicit Vector(size_t size);
    Vector(size_t size, const T& value);
    Vector(const Vector& other);
    Vector& operator=(const Vector& other);
    ~Vector();

    void Clear();

    void PushBack(const T& value);
    void PopBack();

    void Resize(size_t new_size);
    void Resize(size_t new_size, const T& value);
    void Reserve(size_t new_cap);
    void ShrinkToFit();
    void Swap(Vector& other);

    T& operator[](size_t idx);
    const T& operator[](size_t idx) const;

    T& Front();
    T& Back();
    const T& Front() const;
    const T& Back() const;

    bool Empty() const;
    size_t Size() const;
    size_t Capacity() const;
    const T* Data() const;
};

template <class T>
Vector<T>::Vector() : size_(0), capacity_(0), buffer_(nullptr) {
}

template <class T>
Vector<T>::~Vector() {
    delete[] buffer_;
}

template <class T>
void Vector<T>::Fill(size_t start, size_t end, const T& value) {
    for (size_t i = start; i < end; ++i) {
        buffer_[i] = value;
    }
}

template <class T>
void Copy(const T* buff_from, size_t count, T* buff_to) {
    for (size_t i = 0; i < count; ++i) {
        buff_to[i] = buff_from[i];
    }
}

template <class T>
Vector<T>::Vector(size_t size) : size_(size), capacity_(size) {
    buffer_ = new T[size_];
}

template <class T>
Vector<T>::Vector(size_t size, const T& value) : Vector(size) {
    Fill(0, Size(), value);
}

template <class T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    if (&other == this) {
        return *this;
    }

    if (other.Size() > Capacity()) {
        capacity_ = other.capacity_;
        delete[] buffer_;
        buffer_ = new T[Capacity()];
    }

    size_ = other.size_;
    Copy(other.buffer_, Size(), buffer_);
    return *this;
}

template <class T>
Vector<T>::Vector(const Vector& other) : size_(other.Size()), capacity_(other.Capacity()) {
    buffer_ = new T[other.Capacity()];
    Copy(other.buffer_, Size(), buffer_);
}

template <class T>
size_t Vector<T>::Size() const {
    return size_;
}

template <class T>
size_t Vector<T>::Capacity() const {
    return capacity_;
}

template <class T>
const T* Vector<T>::Data() const {
    return buffer_;
}

template <class T>
bool Vector<T>::Empty() const {
    return Size() == 0;
}

template <class T>
T& Vector<T>::operator[](size_t idx) {
    return buffer_[idx];
}

template <class T>
const T& Vector<T>::operator[](size_t idx) const {
    return buffer_[idx];
}
template <class T>
T& Vector<T>::Front() {
    return buffer_[0];
}

template <class T>
T& Vector<T>::Back() {
    return buffer_[Size() - 1];
}

template <class T>
const T& Vector<T>::Front() const {
    return buffer_[0];
}

template <class T>
const T& Vector<T>::Back() const {
    return buffer_[Size() - 1];
}

size_t Min(size_t a, size_t b) {
    return (a < b) ? a : b;
}

template <class T>
void Swap(T& a, T& b) {
    T c = a;
    a = b;
    b = c;
}

template <class T>
void Vector<T>::Swap(Vector<T>& other) {
    ::Swap(buffer_, other.buffer_);
    ::Swap(capacity_, other.capacity_);
    ::Swap(size_, other.size_);
}

template <class T>
void Vector<T>::Clear() {
    size_ = 0;
}

template <class T>
void Vector<T>::BufferReallocation(size_t new_capacity) {
    T* new_buff = (new_capacity == 0) ? nullptr : new T[new_capacity];
    size_ = Min(new_capacity, Size());
    Copy(buffer_, Size(), new_buff);

    delete[] buffer_;
    buffer_ = new_buff;
    capacity_ = new_capacity;
}

template <class T>
size_t Vector<T>::FindCorrectCapacity() {
    return (Capacity() == 0) ? 1 : Capacity() * kIncreaseFactor;
}

template <class T>
void Vector<T>::PushBack(const T& value) {
    if (Size() == Capacity()) {
        BufferReallocation(FindCorrectCapacity());
    }

    buffer_[Size()] = value;
    ++size_;
}

template <class T>
void Vector<T>::PopBack() {
    if (!Empty()) {
        --size_;
    }
}

template <class T>
void Vector<T>::ShrinkToFit() {
    if (capacity_ > size_) {
        BufferReallocation(Size());
    }
}

template <class T>
void Vector<T>::Resize(size_t new_size) {
    if (new_size > Capacity()) {
        BufferReallocation(new_size);
    }

    size_ = new_size;
}

template <class T>
void Vector<T>::Resize(size_t new_size, const T& value) {
    size_t old_size = Size();
    Resize(new_size);
    Fill(old_size, Size(), value);
}

template <class T>
void Vector<T>::Reserve(size_t new_cap) {
    if (new_cap > Capacity()) {
        BufferReallocation(new_cap);
    }
}

template <class T>
bool operator<(const Vector<T>& lhs, const Vector<T>& rhs) {
    for (size_t i = 0; (i < lhs.Size()) && (i < rhs.Size()); ++i) {
        if (lhs[i] > rhs[i]) {
            return false;
        } else if (lhs[i] < rhs[i]) {
            return true;
        }
    }

    return lhs.Size() < rhs.Size();
}

template <class T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs) {
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

template <class T>
bool operator>(const Vector<T>& lhs, const Vector<T>& rhs) {
    return rhs < lhs;
}

template <class T>
bool operator<=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(rhs < lhs);
}

template <class T>
bool operator>=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(lhs < rhs);
}

template <class T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(lhs == rhs);
}

#endif // VECTOR_H

