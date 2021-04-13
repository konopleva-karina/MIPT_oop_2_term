#ifndef ARRAY_H
#define ARRAY_H

#include <cstddef>

template <class T, size_t N>
struct Array {
    T buffer_[N];

    Array() = default;
    Array(const Array &other) = default;

    ~Array() = default;

    size_t Size() const {
        return N;
    }

    bool Empty() const {
        return Size() == 0;
    }

    T& operator[](int idx) {
        return buffer_[idx];
    }

    const T& operator[](size_t i) const {
        return buffer_[i];
    }

    const T& Front() const {
        return buffer_[0];
    }

    T& Front() {
        return buffer_[0];
    }

    const T& Back() const {
        return buffer_[Size() - 1];
    }

    T& Back() {
        return buffer_[Size() - 1];
    }

    T* Data() {
        T* data = buffer_;
        return data;
    }

    void Fill(const T& value) {
        for (size_t i = 0; i < N; ++i) {
            buffer_[i] = value;
        }
    }

    void Swap(Array& other) {
        swap(buffer_, other.buffer_);
    }

    const T& At(size_t n) const {
        return buffer_[n];
    }
};

template<class T, size_t N>
bool operator==(const Array<T, N>& lhs, const Array<T, N>& rhs) {
    for (size_t i = 0; i < lhs.Size(); ++i) {
        if (lhs.buffer_[i] != rhs.buffer_[i]) {
            return false;
        }
    }

    return true;
}

template<class T, size_t N>
bool operator<(const Array<T, N>& lhs, const Array<T, N>& rhs) {
    for (size_t i = 0; i < lhs.Size(); ++i) {
        if (lhs.buffer_[i] >= rhs.buffer_[i]) {
            return false;
        }
    }

    return true;
}

template<class T, size_t N>
bool operator>(const Array<T, N>& lhs, const Array<T, N>& rhs) {
    return !(lhs == rhs || lhs < rhs);
}

#endif //ARRAY_H

