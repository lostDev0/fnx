#pragma once

namespace fnx
{
class heap_pool_allocator_base
{
public:
    heap_pool_allocator_base() = default;
    ~heap_pool_allocator_base() = default;
    virtual void destroy( void* ptr ) = 0;
};

template<typename T, size_t block_size = 1024>
class heap_pool_allocator : public heap_pool_allocator_base
{
public:

    template<typename... TArgs>
    /// @brief Calls the object constructor at next available memory location.
    /// @return T*: pointer to the object memory
    /// @note This function will allocate another chunk if needed based on the id provided.
    T* create( TArgs&& ... args )
    {
        T* addr = alloc();

        if ( nullptr != addr )
        {
            new ( addr ) T( std::forward<TArgs>( args )... );
        }

        return addr;
    }

    /// @brief Calls the object destructor and frees the memory for the next call to create().
    virtual void destroy( void* ptr ) override
    {
        if ( nullptr != ptr )
        {
            auto n_ptr = static_cast<T*>( ptr );
            n_ptr->~T();
            dealloc( n_ptr );
        }
    }

protected:
private:
    union Node
    {
        T _element;
        Node* _next{ nullptr };
    public:
        Node() {}
        ~Node() {}
    };

    /// @brief Allocate another block of objects.
    void expand()
    {
        // allocate a large block of objects for use plus a single pointer for the next block
        Node* block_loc = static_cast<Node*>( calloc( block_size + 1, sizeof( Node ) ) );

        if ( nullptr != block_loc )
        {
            // no next block to point to yet as this will become the current block
            block_loc[0]._next = nullptr;

            if ( nullptr == _block_head )
            {
                // first block to be allocated
                _block_head = block_loc;
            }
            else
            {
                // update the previous block to point to the new block
                _current_block->_next = &block_loc[0];
            }

            _current_block = block_loc;
            // current node becomes the head of the block's node array
            _current_node = &block_loc[1];
            // last node becomes the tail of the block's node array
            _last_node = block_loc + block_size;
            // no free node since we've just made a new block
            _free_node = nullptr;
        }
    }

    T* alloc()
    {
        T* addr = nullptr;

        if ( nullptr != _free_node )
        {
            // already have a free spot, use it
            addr = reinterpret_cast<T*>( _free_node );
            _free_node = _free_node->_next;
        }
        else
        {
            // shouldn't be any nodes left or have never been
            if ( _current_node > _last_node || nullptr == _current_node )
            {
                expand();
            }

            addr = reinterpret_cast<T*>( _current_node++ );
        }

        return addr;
    }

    void dealloc( T* pointer )
    {
        if ( nullptr != pointer )
        {
            Node* node_ptr = reinterpret_cast<Node*>( pointer );
            node_ptr->_next = _free_node;
            _free_node = node_ptr;
        }
    }

    // block head will point to the first allocated block
    Node* _block_head{ nullptr };
    // first word of each block points to the next block
    Node* _current_block{ nullptr };

    // current node to be used for alloc
    Node* _free_node{ nullptr };
    // next free node
    Node* _current_node{ nullptr };
    // last node in the current block
    Node* _last_node{ nullptr };
};
}