#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
/**
 * Thread safe queue that uses a single sequential block of memory in a loop
 * No need to shift elements after deletion!
 */
class CircularBuffer {
public:
    /**
     * Constructor. CircularBuffers have a fixed capacity so initialize it here
     */
    explicit CircularBuffer(size_t capacity) : capacity_(capacity) {}

    /**
     * Push function. Should block if the queue is full so the reader doesn't outrun the filter thread
     */
    void push(T item) {
        std::unique_lock<std::mutex> lock(mutex_);
        // wait until there's room - lambda checks the condition every time we wake, which protects agains spurious wakeups
        not_full_.wait(lock, [this] { return queue_.size() < capacity_ || done_; })
        if (done_) return;
        queue_.push(std::move(item));
        lock.unlock();
        not_empty_.notify_one(); // wake up a waiting consumer
    }  

    /**
     * pop function. block if queue is empty
     */
    bool pop(T& out) {
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [this] { return !queue.empty() || done_; });
        if (queue_.empty()) return false;
        queue_.pop();
        lock.unlock();
        not_full_.notify_one(); // wake up a waiting producer
        return true;
    }

    /**
     * Call this from the producer when there is no more data.
     * Wakes up any consumer stuck waiting so it can exit cleanly instead of hanging
     */
    void close() {
        std::lock_guard<std::mutex> lock(mutex_);
        done_ = true;
        not_empty_.notify_all(); // unblocks all threads currently waiting on condition variable
        not_full_.notify_all(); // same thing
    }
private:
    std::queue<T> queue_;
    size_t capacity_;
    bool done_ = false;
    std::mutex mutex_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;

};