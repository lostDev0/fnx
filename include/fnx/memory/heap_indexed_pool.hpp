#pragma once

#include <vector>

namespace fnx
{
class heap_indexed_pool_base
{
public:
    heap_indexed_pool_base( size_t element_size, size_t num_elements )
        : _element_size{ element_size }
        , _elements_per_block{ num_elements }
        , _element_capacity{ 0u }
    {}
    virtual ~heap_indexed_pool_base()
    {
        for ( auto block : _blocks )
        {
            delete[] block;
        }
        _blocks.clear();
    }

    void expand( size_t index )
    {
        if ( index >= _size )
        {
            if ( index >= _element_capacity )
            {
                reserve( index );
            }
            _size = index;
        }
    }

    void reserve( size_t num )
    {
        while ( _element_capacity < num )
        {
            auto block = new char[_element_size * _elements_per_block];
            _blocks.emplace_back( block );
            _element_capacity += _elements_per_block;
        }
    }

    void* at( size_t index )
    {
        assert( index < _size );
        return _blocks[index / _elements_per_block] + ( index % _elements_per_block ) * _element_size;
    }

    const void* at( size_t index ) const
    {
        assert( index < _size );
        return _blocks[index / _elements_per_block] + ( index % _elements_per_block ) * _element_size;
    }

    void* operator[]( size_t index )
    {
        return this->at( index );
    }

    virtual void destroy( size_t index ) = 0;

    auto capacity()
    {
        return _element_capacity;
    }
    auto num_blocks()
    {
        return _blocks.size();
    }
protected:
    heap_indexed_pool_base() = delete;

    std::vector<char*> _blocks;
    size_t _element_size{ 0u };
    size_t _elements_per_block{ 0u };
    size_t _element_capacity{ 0u };
    size_t _size{ 0u };
};

template<typename T, size_t block_size = 8192>
/// @brief Heap based pools of memory that are accessed and destroyed via index.
class heap_indexed_pool : public heap_indexed_pool_base
{
public:
    heap_indexed_pool()
        : heap_indexed_pool_base( sizeof( T ), block_size )
    {}
    virtual ~heap_indexed_pool() {}

    virtual void destroy( size_t index ) override
    {
        assert( index < _size );
        auto ptr = static_cast<T*>( at( index ) );
        ptr->~T();
    }

    T* create()
    {
        expand( _size + 1 );
        return create( _size - 1 );
    }

    template<typename... TArgs>
    T* create( size_t index, TArgs&& ... args )
    {
        expand( index + 1 );
        T* addr = static_cast<T*>( at( index ) );
        new ( addr ) T( std::forward<TArgs>( args )... );
        return addr;
    }
};
}