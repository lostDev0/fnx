#pragma once

namespace fnx
{
    class dispatcher_interface
    {
    public:
        dispatcher_interface() {}
        virtual ~dispatcher_interface() {}

        virtual void update(double delta) = 0;
    };

    template<typename T>
    /// @brief Used to send events to the engine features.
    class dispatcher : public dispatcher_interface
    {
        template<typename U>
        /// @brief Used internally to store events that can be triggered in the future or now.
        struct message
        {
            message() {}
            ~message() {}
            message(const U& payload, bool reverse, double delay)
                : _payload{ payload }
                , _reverse{ reverse }
                , _time_left{ delay }
            {}
            U _payload{};
            bool _reverse{ false };
            double _time_left{ 0.0 };
        };

    public:

        using subscriber = fnx::function_ref<bool(const T&)>;

        dispatcher() = default;
        ~dispatcher() = default;

        /// @brief Add an event to the message queue. This event can be delayed or sent in reverse subscriber order.
        void trigger(const T& event, bool reverse, double delay)
        {
            _messages.emplace_back(event, reverse, delay);
        }

        /// @brief Dispatch an event immediately, blocking the calling thread.
        void trigger_immediate(const T& event, bool reverse)
        {
            reverse ? emit_reverse(event) : emit(event);
        }

        /// @brief Subscribe for event notifications of a certain payload type.
        void subscribe(const subscriber& func)
        {
            auto it_end = _subscribers.end();
            auto it = std::find(_subscribers.begin(), it_end, func);
            if (it == it_end)
            {
                _subscribers.emplace_back(func);
            }
        }

        /// @brief Unsubscribe from event notifications of a certain payload type.
        void unsubscribe(const subscriber& func)
        {
            auto it_end = _subscribers.end();
            auto it = std::find(_subscribers.begin(), it_end, func);
            if (it != it_end)
            {
                _subscribers.erase(it);
            }
        }

        /// @brief Emit all queued events who's delay has expired.
        void update(double delta) override
        {
            auto i = _messages.size();
            while (i > 0)
            {
                --i;
                auto message = _messages.pop();
                message._time_left -= delta;
                bool expired = message._time_left <= 0.0;
                if (!expired)
                {
                    // push the event to the end of the queue
                    _messages.push(message);
                    continue;
                }
                message._reverse ? emit_reverse(message._payload) : emit(message._payload);
            }
        }

    private:
        std::vector<subscriber> _subscribers;
        fnx::ring_buffer<message<T>> _messages;

        void emit(const T& event) const
        {
            for (const auto& sub : _subscribers)
            {
                if (sub(event))
                {
                    // was absorbed
                    break;
                }
            }
        }

        void emit_reverse(const T& event) const
        {
            auto it_end = _subscribers.rend();
            for (auto it = _subscribers.rbegin(); it != it_end; ++it)
            {
                if ((*it)(event))
                {
                    // was absorbed
                    break;
                }
            }
        }
    };
}
