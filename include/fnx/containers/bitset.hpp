#pragma once

namespace fnx
{
template<size_t total>
struct bitset
{
    bitset() = default;
    ~bitset() = default;
    bitset( const bitset<total>& other )
    {
        memcpy( _bits, other._bits, sizeof( _bits ) );
    }

    inline bool is_set( size_t index ) const
    {
        auto div = index / _bits_per_section;
        auto l_index = index % _bits_per_section;
        return _bits[div] >> l_index & 1;
    }

    inline auto& set( size_t index )
    {
        auto div = index / _bits_per_section;
        auto l_index = index % _bits_per_section;
        _bits[div] |= 1 << l_index;
        return *this;
    }

    inline auto& unset( size_t index )
    {
        auto div = index / _bits_per_section;
        auto l_index = index % _bits_per_section;
        _bits[div] &= ~( 1UL << l_index );
        return *this;
    }

    inline auto& toggle( size_t index )
    {
        auto div = index / _bits_per_section;
        auto l_index = index % _bits_per_section;
        _bits[div] ^= 1 << l_index;
        return *this;
    }

    inline auto& set( size_t index, int x )
    {
        auto div = index / _bits_per_section;
        auto l_index = index % _bits_per_section;
        _bits[div] ^= ( -x ^ _bits[div] ) & ( 1UL << l_index );
        return *this;
    }

    inline auto operator[]( size_t index ) const
    {
        return is_set( index );
    }

    inline void reset()
    {
        memset( _bits, 0, sizeof( _bits ) );
    }

    inline void set()
    {
        memset( _bits, 1, sizeof( _bits ) );
    }

    inline auto size() const
    {
        return _total;
    }

    inline auto test( size_t index ) const
    {
        if ( index >= _total )
        {
            return false;
        }
        return is_set( index );
    }

    auto& operator=( const fnx::bitset<total>& other )
    {
        memcpy( _bits, other._bits, sizeof( _bits ) );
        return *this;
    }

    auto operator==( const fnx::bitset<total>& other ) const
    {
        return std::memcmp( _bits, other._bits, sizeof( _bits ) ) == 0;
    }

    auto operator|( const fnx::bitset<total>& other ) const
    {
        fnx::bitset<total> ret = *this;

        for ( size_t i = 0; i < _divisions; i++ )
        {
            ret._bits[i] |= other._bits[i];
        }

        return ret;
    }

    auto count() const
    {
        // Brian Kernighan Algorithm
        unsigned int count{ 0 };
        for ( size_t i = 0; i < _divisions; i++ )
        {
            auto n = _bits[i];
            while ( n )
            {
                n &= ( n - 1 );
                count++;
            }
        }
        return count;
    }

private:

    static const size_t _total = total;
    static const size_t _divisions = total / ( sizeof( uint32_t ) * 8 ) + 1;
    static const size_t _bits_per_section = sizeof( uint32_t ) * 8;

    uint32_t _bits[total / ( sizeof( uint32_t ) * 8 ) + 1] { 0 }; // add 1 to account for truncation on int division
};

template<unsigned int A, unsigned int B>
bool includes( const fnx::bitset<A>& set, const fnx::bitset<B>& subset )
{
    bool ret = true;
    if ( subset.size() <= set.size() && subset.count() <= set.count() )
    {
        auto matches = 0u;
        auto total = subset.count();
        for ( auto i = 0u; ( i < subset.size() ) && ret && ( matches != total ); ++i )
        {
            if ( subset[i] && !set[i] )
            {
                // early out
                ret = false;
            }
            else if ( subset[i] && set[i] )
            {
                ++matches;
            }
        }
    }
    else
    {
        ret = false;
    }
    return ret;
}
}