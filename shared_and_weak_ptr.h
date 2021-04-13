#ifndef SHARED_PTR_SHARED_PTR_H
#define SHARED_PTR_SHARED_PTR_H

#include <cstdlib>
#include <utility>
#include <exception>
#include <string>

class BadWeakPtr : public std::exception {
    std::string message_;

public:
    BadWeakPtr() = default;

    explicit BadWeakPtr(const char* message) : message_(message) {
    }

    explicit BadWeakPtr(std::string message) : message_(std::move(message)) {
    }

    const char* what() const noexcept override {
        return message_.c_str();
    }
};

template<class T>
class WeakPtr;

struct Counter {
    size_t cnt = 0;
    size_t weak_cnt = 0;

    Counter(size_t cnt, size_t weak_cnt) : cnt(cnt), weak_cnt(weak_cnt) {
    }
};

template<class T>
class SharedPtr {
    T* ptr_ = nullptr;
    Counter* counters_ = nullptr;

public:
    SharedPtr() = default;

    SharedPtr(T* ptr) : ptr_(ptr), counters_(ptr ? new Counter{1, 0} : nullptr) {
    }

    SharedPtr(const SharedPtr& other) : ptr_(other.ptr_), counters_(other.counters_) {
        if (ptr_ != nullptr) {
            ++counters_->cnt;
        }
    }

    SharedPtr(SharedPtr&& other) noexcept : ptr_(other.ptr_), counters_(other.counters_) {
        other.ptr_ = nullptr;
        other.counters_ = nullptr;
    }

    SharedPtr(const WeakPtr<T>& weak_ptr) : ptr_(weak_ptr.ptr_), counters_(weak_ptr.counters_) {
        if (weak_ptr.Expired()) {
            throw BadWeakPtr{};
        }

        if (ptr_) {
            ++counters_->cnt;
        }
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (this == &other) {
            return *this;
        }

        SharedPtr tmp(other);
        Swap(tmp);

        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        SharedPtr tmp(std::move(other));
        Swap(tmp);

        return *this;
    }

    ~SharedPtr<T>() {
        if (counters_ == nullptr) {
            return;
        }

        --counters_->cnt;
        if (!counters_->cnt) {
            delete ptr_;
            if (!counters_->weak_cnt) {
                delete counters_;
            }
        }
    }

    size_t UseCount() const {
        return counters_ ? counters_->cnt : 0 ;
    }

    T* Get() const {
        return ptr_;
    }

    T& operator*() const {
        return *ptr_;
    }

    explicit operator bool() const noexcept {
        return ptr_;
    }

    void Swap(SharedPtr& other) {
        std::swap(ptr_, other.ptr_);
        std::swap(counters_, other.counters_);
    }


    void Reset(T* ptr = nullptr) {
        SharedPtr tmp(std::move(ptr));
        Swap(tmp);
    }

    T* operator->() const {
        return ptr_;
    }

    friend class WeakPtr<T>;
};


template<class T>
class WeakPtr {
    T* ptr_ = nullptr;
    Counter* counters_ = nullptr;

public:
    WeakPtr() = default;

    WeakPtr(const SharedPtr<T>& shared_ptr) : ptr_(shared_ptr.Get()), counters_(shared_ptr.counters_) {
        if (ptr_) {
            ++counters_->weak_cnt;
        }
    }

    WeakPtr(const WeakPtr& other) : ptr_(other.ptr_), counters_(other.counters_) {
        if (ptr_) {
            ++counters_->weak_cnt;
        }
    }

    WeakPtr(WeakPtr&& other) noexcept
            : ptr_(other.ptr_),
              counters_(other.counters_ ) {
        other.counters_ = nullptr;
        other.ptr_ = nullptr;
    }

    WeakPtr& operator=(const WeakPtr& other) {
        if (this == &other) {
            return *this;
        }

        WeakPtr tmp(other);
        Swap(tmp);

        return *this;
    }

    WeakPtr& operator=(WeakPtr&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        WeakPtr tmp(other);
        Swap(tmp);

        return *this;
    }

    ~WeakPtr() {
        if (!counters_) {
            return;
        }
        --counters_->weak_cnt;
        if (counters_->cnt == 0 && counters_->weak_cnt == 0) {
            delete counters_;
        }
    }

    void Swap(WeakPtr& other) {
        std::swap(ptr_, other.ptr_);
        std::swap(counters_, other.counters_);
    }

    size_t UseCount() const {
        return counters_ ? counters_->cnt : 0;
    }

    void Reset() {
        WeakPtr tmp{};
        Swap(tmp);
    }

    bool Expired() const {
        return ptr_ == nullptr || UseCount() == 0;
    }

    SharedPtr<T> Lock() const {
        return Expired() ? nullptr : SharedPtr<T>(*this);
    }

    friend class SharedPtr<T>;
};

#endif //SHARED_PTR_SHARED_PTR_H

