#pragma once

namespace fnx
{
/// @brief Manages unique identifiers. Valid idents are from 1 to the max_handles provided at creation.
/// @note 0 is considered invalid. If using this class for indexing, adjust by 1
template<typename T, size_t max_handles, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class id_manager
{
public:

    static constexpr T invalid_id = max_handles;
    static constexpr size_t max = max_handles;

    /// @brief Construct the handle_id_t manager with a specific maximum number of handles
    id_manager()
        : _available()
    {
        _available.reserve( max_handles );
        assert( max_handles > 0 );
        for ( auto id = max_handles; id > 0; --id )
        {
            _available.emplace_back( static_cast<T>( id - 1 ) );
        }
    }

    ~id_manager() {}

    /// @brief Return a handle if one is available.
    /// @return 0 indicates an invalid id
    T get_handle()
    {
        auto ret = invalid_id;
        if ( !_available.empty() )
        {
            auto id = _available.back();
            _available.pop_back();
            _used.set( id, 1 );
            ret = id;
        }

        return ret;
    }

    /// @brief Reclaim a handle to be used again.
    /// @param[in] id: id to reuse
    void reclaim( T id )
    {
        if ( max_handles > id &&	// must have enough handles
                1 == _used[id] )		// must be in use
        {
            _available.push_back( id );
            _used.set( id, 0 );
        }
    }

    /// @brief Reclaim all handles to be used again.
    void reclaim_all()
    {
        _available.clear();
        _used.reset();
        for ( auto id = max_handles; id > 0; --id )
        {
            _available.emplace_back( id - 1 );
        }
    }

    /// @brief Sorts the avaiable handles. This is useful to ensure the lowest number is always
    ///		returned to the caller.
    void sort()
    {
        std::qsort( _available.begin(), _available.end(), std::greater<T>() );
    }

    /// @brief Return the number of available handles.
    size_t num_available()
    {
        return _available.size();
    }

    /// @brief Return the nubmer of used handles.
    size_t num_used() const
    {
        return max_handles - _available.size();
    }

    /// @brief Return all available ids.
    auto& available()
    {
        return _available;
    }

    /// @brief Return whether a specific id is in use.
    /// @note The id is the location within the bitset.
    bool in_use( T id ) const
    {
        return id != invalid_id ? _used[id] : false;
    }

    /// @brief Return all used ids.
    /// @note These ids are the location within the bitset
    const auto& in_use() const
    {
        return _used;
    }
private:
    std::vector<T> _available;		/// list of all currently available ids
    fnx::bitset<max_handles> _used;
};
}