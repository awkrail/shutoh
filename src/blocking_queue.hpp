#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <optional>
#include <chrono>

template <typename T>
class BlockingQueue {
    public:
        BlockingQueue(size_t max_size) : max_size{max_size} {}

    void push(const T& item) {
        std::unique_lock<std::mutex> lock(mutex);
        cond_producer.wait(lock, [this]() { return queue.size() < max_size; });
        
        queue.push(item);
        cond_consumer.notify_one();
    }

    T get() {
        std::unique_lock<std::mutex> lock(mutex);
        cond_consumer.wait(lock, [this]() { return !queue.empty(); });

        T item = queue.front();
        queue.pop();
        cond_producer.notify_one();
        return item;
    }

    size_t size() {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.size();
    }

    bool empty() {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.empty();
    }

    private:
        std::queue<T> queue;
        size_t max_size;
        std::mutex mutex;
        std::condition_variable cond_producer;
        std::condition_variable cond_consumer;
};

#endif