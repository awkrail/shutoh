#ifndef FIXED_SIZE_DEQUE_H
#define FIXED_SIZE_DEQUE_H

#include <deque>

template <typename T>
class FixedSizeDeque {
    public:
        FixedSizeDeque(size_t max_size) : max_size_{max_size} {}
        
        void push(const T& item) {
            while(deque_.size() >= max_size_) /* WATCH: >? */
                deque_.pop_front();  
            deque_.push_back(item);
        }

        size_t size() const {
            return deque_.size();
        }

        T operator[](size_t index) const {
            return deque_[index];
        }

    private:
        std::deque<T> deque_;
        const size_t max_size_;
};

#endif