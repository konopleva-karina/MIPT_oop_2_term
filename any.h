#ifndef ANY_H
#define ANY_H

#include <memory>
#include <exception>

class BadAnyCast : public std::bad_cast {
    std::string message_;
public:
    BadAnyCast() = default;

    explicit BadAnyCast(std::string message) : message_(std::move(message)) {
    }

    const char* what() const noexcept override {
        return message_.c_str();
    }
};

class Base {
public:
    Base() = default;
    virtual ~Base() = default;
    virtual std::shared_ptr<Base> Clone() const = 0;
};

template<typename T>
class Derived : public Base {
public:
    T value;

    Derived(const T& value) : value(value) {
    }

    ~Derived() = default;

    std::shared_ptr<Base> Clone() const  override {
        return std::make_shared<Derived<T>>(value);
    }
};

class Any {
    std::shared_ptr<Base> base_ptr_;

public:

    Any() : base_ptr_(nullptr) {
    }

    Any(const Any& other) : base_ptr_(other.HasValue() ? other.base_ptr_->Clone() : nullptr) {
    }

    template<typename T>
    Any(const T& value) : base_ptr_(std::make_shared<Derived<T>>(value)) {
    }

    Any(Any&& other) noexcept {
        if (!other.HasValue()) {
            base_ptr_ = nullptr;
            return;
        }
        base_ptr_ = other.base_ptr_->Clone();
        other.base_ptr_ = nullptr;
    }

    ~Any() {
        Reset();
    }

    void Reset() {
        base_ptr_.reset();
    }

    bool HasValue() const {
        return base_ptr_ != nullptr;
    }

    Any& operator=(const Any& other) {
        if (this != &other) {
            Reset();
            base_ptr_ = other.base_ptr_->Clone();
        }
        return *this;
    }

    template<typename U>
    Any& operator=(const U& rhs) {
        Reset();
        base_ptr_ = std::make_shared<Derived<U>>(rhs);
        return *this;
    }

    Any& operator=(Any&& rhs) noexcept {
        if (this != &rhs) {
            Reset();
            base_ptr_ = rhs.base_ptr_;
            rhs.base_ptr_ = nullptr;
        }
        return *this;
    }

    void Swap(Any& other) {
        std::swap(this->base_ptr_, other.base_ptr_);
    }

    template<class T>
    friend T any_cast(const Any& a);
};

template<class T>
T any_cast(const Any&   a) {
    if (std::dynamic_pointer_cast<Derived<T>>(a.base_ptr_)) {
        return std::dynamic_pointer_cast<Derived<T>>(a.base_ptr_)->value;
    }
    throw BadAnyCast{};
}


#endif // ANY_H

