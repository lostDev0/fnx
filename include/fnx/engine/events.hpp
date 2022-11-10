#pragma once

#include <unordered_map>
#include <memory>

namespace fnx
{
class event_manager
{
public:

    /// @brief Queues events to be handled upon system update.
    template<typename T, typename... Args>
    void emit( bool reverse, double delay, Args&& ... args )
    {
        emit( T{ std::forward<Args>( args )... }, reverse, delay );
    }

    template<typename T>
    /// @brief Trigger an event to be queued.
    void emit( const T& payload, bool reverse = false, double delay = 0.0 )
    {
        FNX_INFO( fnx::to_string( payload ) );
        auto& d = get_dispatcher<T>();
        d.trigger( payload, reverse, delay );
    }

    template<typename T, typename... Args>
    void emit_immediately( const T& payload, bool reverse = false )
    {
        auto& d = get_dispatcher<T>();
        d.trigger_immediate( payload, reverse );
    }

    template<typename T, typename... Args>
    /// @brief Fire an event without queueing it.
    void emit_immediately( bool reverse, Args&& ... args )
    {
        emit_immediately( T{ std::forward<Args>( args )... }, reverse );
    }

    /// @brief Command all dispatchers to fire their events.
    void update( double delta_in_seconds )
    {
        for ( auto& d : _dispatchers )
        {
            d.second->update( delta_in_seconds );
        }
    }

    template<typename T>
    /// @brief Register a callback function to be called when events of a given type are triggered.
    void subscribe( const typename fnx::dispatcher<T>::subscriber& f )
    {
        auto& d = get_dispatcher<T>();
        d.subscribe( f );
    }

    template<typename T>
    /// @brief Remove callback function for the event type.
    void unsubscribe( const typename fnx::dispatcher<T>::subscriber& f )
    {
        auto& d = get_dispatcher<T>();
        d.unsubscribe( f );
    }

private:
    std::unordered_map<unsigned int, std::unique_ptr<dispatcher_interface>> _dispatchers;

    template<typename T>
    fnx::dispatcher<T>& get_dispatcher()
    {
        static const auto type = get_unique_dispatcher_type();
        if ( _dispatchers[type] == nullptr )
        {
            // need to allocate a new dispatcher
            _dispatchers[type] = std::make_unique<dispatcher<T>>();
        }
        fnx::dispatcher<T>& dispatcher = *( static_cast<fnx::dispatcher<T>*>( _dispatchers[type].get() ) );
        return dispatcher;
    }

    static auto get_unique_dispatcher_type()
    {
        static unsigned int type = 0u;
        return type++;
    }
};

#define FNX_EMIT(e) { auto [emitter,_99] = fnx::singleton<fnx::event_manager>::acquire(); emitter.emit(e); }
}