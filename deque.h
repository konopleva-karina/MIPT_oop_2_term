#include <cstddef>

//================ Page ================//

template <class T, size_t N>
class Page {
    T buffer_[N];
    size_t size_;
    size_t begin_;

public:
    Page() : size_(0), begin_(0) {
    }

    Page(const Page& other) = default;
    Page& operator=(const Page& other) = default;
    ~Page() = default;

    T& operator[](size_t idx) {
        return buffer_[begin_ + idx];
    }

    const T& operator[](size_t idx) const {
        return buffer_[begin_ + idx];
    }

    const T& Front() const {
        return buffer_[begin_];
    }

    T& Front() {
        return buffer_[begin_];
    }

    const T& Back() const {
        return buffer_[begin_ + size_ - 1];
    }

    T& Back() {
        return buffer_[begin_ + size_ - 1];
    }

    bool Empty() const {
        return Size() == 0;
    }

    size_t Size() const {
        return size_;
    }

    bool Full() const {
        return Size() == N;
    }

    bool IsBack() const {
        return Empty() || (begin_ == 0 && !Full());
    }

    bool IsFront() const {
        return Empty() || (begin_ != 0 && !Full());
    }

    void PushBack(const T& val) {
        if (!IsBack()) {
            return;
        }

        if (Empty()) {
            begin_ = 0;
        }

        buffer_[size_++] = val;
    }

    void PushFront(const T& val) {
        if (!IsFront()) {
            return;
        }

        if (Empty()) {
            begin_ = N;
        }

        buffer_[--begin_] = val;
        ++size_;
    }

    void PopBack() {
        if (Empty()) {
            return;
        }

        --size_;
    }

    void PopFront() {
        if (Empty()) {
            return;
        }

        --size_;
        ++begin_;
    }

    void Clear() {
        size_ = 0;
    }
};

//================ CircularBuffer ================//

template <class U>
void Swap(U& a, U& b) {
    U c = a;
    a = b;
    b = c;
}

template <class U>
class CircularBuffer {
    U* buffer_;
    size_t capacity_;
    size_t begin_;
    size_t end_;
    size_t size_;

    const static int kIncreaseFactor = 2;


    void Copy(const CircularBuffer& other) {
        Clear();
        for (size_t i = 0; i < other.Size(); ++i) {
            PushBack(other[i]);
        }
    }

public:
    CircularBuffer()
            : buffer_(nullptr),
              capacity_(0),
              begin_(0),
              end_(0),
              size_(0) {
    }

    explicit CircularBuffer(size_t count) : CircularBuffer() {
        capacity_ = count;
        if (count != 0) {
            buffer_ = new U[count];
        }
    }

    CircularBuffer(const CircularBuffer& other)
            : CircularBuffer() {
        Copy(other);
    }

    CircularBuffer& operator=(const CircularBuffer& other) {
        if (this == &other) {
            return *this;
        }

        Copy(other);
    }

    ~CircularBuffer() {
        delete[] buffer_;
    }

    U& operator[](size_t idx) {
        return buffer_[(begin_ + idx) % Capacity()];
    }

    const U& operator[](size_t idx) const {
        return buffer_[(begin_ + idx) % Capacity()];
    }

    const U& Front() const {
        return buffer_[begin_];
    }

    U& Front() {
        return buffer_[begin_];
    }

    const U& Back() const {
        return buffer_[end_];
    }

    U& Back() {
        return buffer_[end_];
    }

    size_t Size() const {
        return size_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    bool Empty() const {
        return Size() == 0;
    }

    void PushFront(const U& val) {
        if (Empty()) {
            if (Capacity() == 0) {
                Reserve(1);
            }
            begin_ = 0;
            end_ = 0;
        } else {
            if (Size() == Capacity()) {
                Reserve(Capacity() * kIncreaseFactor);
            }
            begin_ = (begin_ - 1 + Capacity()) % Capacity();
        }

        buffer_[begin_] = val;
        ++size_;
    }

    void PushBack(const U& val) {
        if (Empty()) {
            if (Capacity() == 0) {
                Reserve(1);
            }
            begin_ = 0;
            end_ = 0;
        } else {
            if (Size() == Capacity()) {
                Reserve(Capacity() * kIncreaseFactor);
            }
            end_ = (end_ + 1) % Capacity();
        }

        buffer_[end_] = val;
        ++size_;
    }

    void PopBack() {
        if (!Empty()) {
            end_ = (end_ - 1 + Capacity()) % Capacity();
            --size_;
        }
    }

    void PopFront() {
        if (!Empty()) {
            begin_ = (begin_ + 1) % Capacity();
            --size_;
        }
    }

    void Clear() {
        size_ = 0;
        end_ = begin_;
    }

    void Reserve(size_t new_cap) {
        if (new_cap <= Capacity()) {
            return;
        }

        U* new_buffer = new U[new_cap];
        for (size_t i = 0; i < Size(); ++i) {
            new_buffer[i] = buffer_[(begin_ + i) % Capacity()];
        }

        capacity_ = new_cap;
        begin_ = 0;
        end_ = begin_ + Size() - 1;

        delete[] buffer_;
        buffer_ = new_buffer;
    }

    void Swap(CircularBuffer<U>& other) {
        ::Swap(buffer_, other.buffer_);
        ::Swap(capacity_, other.capacity_);
        ::Swap(begin_, other.begin_);
        ::Swap(end_, other.end_);
        ::Swap(size_, other.size_);
    }
};

//================ Deque ================//

template <class T>
class Deque {
    const static size_t kPageSize = 100;

    CircularBuffer<Page<T, kPageSize>*> cb_;

public:
    Deque() : cb_() {
    }

    Deque(const Deque& other) : Deque() {
        size_t size_deque = other.Size();
        for (size_t i = 0; i < size_deque; ++i) {
            PushBack(other[i]);
        }
    }

    Deque& operator=(const Deque& other) {
        if (&other == this) {
            return *this;
        }

        for (size_t i = 0; i < cb_.Size(); ++i) {
            delete cb_[i];
        }

        cb_.Clear();

        size_t size_deque = other.Size();
        for (size_t i = 0; i < size_deque; ++i) {
            PushBack(other[i]);
        }
    }

    ~Deque() {
        for (size_t i = 0; i < cb_.Size(); ++i) {
            delete cb_[i];
        }
    }

    T& operator[](size_t idx) {
        if (idx < cb_.Front()->Size()) {
            return (*cb_.Front())[idx];
        }

        size_t page_idx = (idx - cb_.Front()->Size()) / kPageSize;
        return (*cb_[page_idx + 1])[(idx - cb_.Front()->Size()) % kPageSize];
    }

    const T& operator[](size_t idx) const {
        if (idx < cb_.Front()->Size()) {
            return (*cb_.Front())[idx];
        } else {
            size_t page_idx = (idx - cb_.Front()->Size()) / kPageSize;
            return (*cb_[page_idx + 1])[(idx - cb_.Front()->Size()) % kPageSize];
        }
    }

    size_t Size() const {
        size_t res = 0;
        for (size_t i = 0; i < cb_.Size(); ++i) {
            res += cb_[i]->Size();
        }

        return res;
    }

    void Swap(Deque& other) {
        cb_.Swap(other.cb_);
    }

    void PushBack(const T& value) {
        if (cb_.Empty() || !(cb_.Back()->IsBack())) {
            cb_.PushBack(new Page<T, kPageSize>);
        }

        cb_.Back()->PushBack(value);
    }

    void PushFront(const T& value) {
        if (cb_.Empty() || !(cb_.Front()->IsFront())) {
            cb_.PushFront(new Page<T, kPageSize>);
        }

        cb_.Front()->PushFront(value);
    }

    void PopBack() {
        if (cb_.Empty()) {
            return;
        }

        cb_.Back()->PopBack();
        if (cb_.Back()->Empty()) {
            delete cb_.Back();
            cb_.PopBack();
        }
    }

    void PopFront() {
        if (cb_.Empty()) {
            return;
        }

        cb_.Front()->PopFront();
        if (cb_.Front()->Empty()) {
            delete cb_.Front();
            cb_.PopFront();
        }
    }

    void Clear() {
        if (Size() == 0) {
            return;
        }

        for (size_t i = 0; i < cb_.Size(); ++i) {
            delete cb_[i];
        }

        cb_.Clear();
    }
};

