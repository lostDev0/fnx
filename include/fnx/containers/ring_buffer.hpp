#pragma once

#include <mutex>
#include <array>

namespace fnx
{
    template<typename Type, const unsigned int max=256>
    /// @brief Circular buffer of objects. Behaves much like a vector, however, the contents are not shifted when
    ///		items are removed from the container.
    struct ring_buffer
    {
    public:
        ring_buffer() = default;
        ~ring_buffer() = default;

        /// @brief Initializes the ring buffer
        /// @param lst collection of values
        /// @note Does not check that the lst will fit in the declared maximum size
        ring_buffer(const std::initializer_list<Type>& lst)
        {
            for (const auto& l : lst)
            {
                emplace_back(l);
            }
        }

        template<typename... TypeArgs>
        /// @brief Inline construction of an object within the buffer.
        bool emplace_back(TypeArgs&&... args)
        {
            Type obj{ std::forward<TypeArgs>(args)... };
            return push(obj);
        }

        /// @brief Copy construction of an object within the buffer.
        bool push(const Type& item)
        {
            if (_num < max)
            {
                std::scoped_lock lock(_mutex);
                _buffer[_tail] = item;
                _tail = (_tail + 1u) % max;
                _num++;
                return true;
            }
            return false;
        }

        /// @brief Returns a copy of the next object within the buffer.
        Type pop()
        {
            Type ret;
            if (_num > 0u)
            {
                std::scoped_lock lock(_mutex);
                // at least one at head
                ret = _buffer[_head];
                _head = (_head + 1u) % max;
                _num--;
            }
            return ret;
        }

        /// @brief Return the number of objects within the buffer.
        auto size()
        {
            return _num;
        }

        /// @brief Destroy all objects within the buffer.
        void clear()
        {
            std::scoped_lock lock(_mutex);
            _head = 0u;
            _tail = 0u;
            _num = 0u;
            std::fill(std::begin(_buffer), std::end(_buffer), Type{});
        }

    protected:
    private:
        std::mutex _mutex;
        std::array<Type, max> _buffer;
        unsigned int _head{ 0u };
        unsigned int _tail{ 0u };
        unsigned int _num{ 0u };
    };
}
