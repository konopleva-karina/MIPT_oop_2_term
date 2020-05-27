#ifndef UNIQUE_PTR_UNIQUE_PTR_H
#define UNIQUE_PTR_UNIQUE_PTR_H

#include <utility>

template<class T>
class UniquePtr {
    T* ptr_;

public:
    UniquePtr() : ptr_(nullptr) {
    }

    explicit UniquePtr(T* ptr) : ptr_(ptr){
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;
    UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    UniquePtr& operator=(T* other_ptr) {
        if (!(*this == other_ptr)) {
            Reset(other_ptr);
        }
        return *this;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        delete ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
        return *this;
    }

    void Swap(UniquePtr<T>& other) {
        std::swap(ptr_, other.ptr_);
    }

    T* Get() const {
        return ptr_;
    }

    T& operator*() const {
        return *ptr_;
    }

    const T* operator->() const {
        return ptr_;
    }

    explicit operator bool() const {
        return ptr_ != nullptr;
    }

    T* Release() {
        T* old_ptr = ptr_;
        ptr_ = nullptr;
        return old_ptr;
    }

    void Reset(T* ptr = nullptr) {
        delete ptr_;
        ptr_ = ptr;
    }

    ~UniquePtr() {
        delete ptr_;
    }
};

template<class T>
bool operator==(const UniquePtr<T>& lhs, const UniquePtr<T>& rhs) {
    return lhs.Get() == rhs.Get();
}

template<class T>
bool operator !=(const UniquePtr<T>& lhs, const UniquePtr<T>& rhs) {
    return !(lhs == rhs);
}


#endif //UNIQUE_PTR_UNIQUE_PTR_H


