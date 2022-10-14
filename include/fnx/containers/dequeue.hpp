#pragma once

#include <mutex>
#include <deque>
#include <condition_variable>
#include <cassert>

namespace fnx
{
    /// @brief A thread safe wrapper for the std::queue
    template<typename T>
    class dequeue
    {
    public:
        dequeue() = default;
        virtual ~dequeue()
        {
            clear();
        }
        dequeue(const fnx::dequeue<T>&) = delete;

        const T& front()
        {
            std::scoped_lock lock(_mutex);
            return _queue.front();
        }

        const T& back()
        {
            std::scoped_lock lock(_mutex);
            return _queue.back();
        }

        T pop_front()
        {
            std::scoped_lock lock(_mutex);
            auto n = std::move(_queue.front());
            _queue.pop_front();
            return n;
        }

        T pop_back()
        {
            std::scoped_lock lock(_mutex);
            auto n = std::move(_queue.back());
            _queue.pop_back();
            return n;
        }

        void push_back(const T& other)
        {
            std::scoped_lock lock(_mutex);
            _queue.emplace_back(std::move(other));
            _cvBlock.notify_one();
        }

        void push_front(const T& other)
        {
            std::scoped_lock lock(_mutex);
            _queue.emplace_front(std::move(other));
            _cvBlock.notify_one();
        }

        template<typename... Ts>
        void emplace_front(Ts&&... args)
        {
            std::scoped_lock lock(_mutex);
            _queue.emplace_front(std::forward<Ts>(args)...);
            _cvBlock.notify_one();
        }

        template<typename... Ts>
        void emplace_back(Ts&&... args)
        {
            std::scoped_lock lock(_mutex);
            _queue.emplace_back(std::forward<Ts>(args)...);
            _cvBlock.notify_one();
        }

        auto empty()
        {
            std::scoped_lock lock(_mutex);
            return _queue.empty();
        }

        void clear()
        {
            std::scoped_lock lock(_mutex);
            _queue.clear();
        }

        auto size()
        {
            std::scoped_lock lock(_mutex);
            return _queue.size();
        }

        void wait_for_data()
        {
            while (empty())
            {
                std::unique_lock<std::mutex> lock(_block);
                _cvBlock.wait(lock);
            }
        }

    protected:
    private:
        std::mutex _mutex;
        std::deque<T> _queue;
        std::condition_variable _cvBlock;
        std::mutex _block;
    };
}